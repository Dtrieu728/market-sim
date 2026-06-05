#include "AlpacaFeed.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <atomic>

extern std::atomic<bool> g_running;

AlpacaFeed::AlpacaFeed(const std::string& apiKey,
                       const std::string& apiSecret,
                       const std::string& symbol,
                       PriceQueue& queue,
                       Logger& logger)
    : apiKey_(apiKey), apiSecret_(apiSecret), symbol_(symbol),
      queue_(queue), logger_(logger), running_(false) {

    client_.clear_access_channels(websocketpp::log::alevel::all);
    client_.clear_error_channels(websocketpp::log::elevel::all);

    client_.init_asio();
    client_.set_tls_init_handler(
        [this](websocketpp::connection_hdl hdl) {
            return onTlsInit(hdl);
        });
    client_.set_open_handler(
        [this](websocketpp::connection_hdl hdl) {
            onOpen(hdl);
        });
    client_.set_message_handler(
        [this](websocketpp::connection_hdl hdl, WsClient::message_ptr msg) {
            onMessage(hdl, msg);
        });
    client_.set_close_handler(
        [this](websocketpp::connection_hdl hdl) {
            onClose(hdl);
        });
    client_.set_fail_handler(
        [this](websocketpp::connection_hdl hdl) {
            onFail(hdl);
        });
}

SslContext AlpacaFeed::onTlsInit(websocketpp::connection_hdl) {
    auto ctx = websocketpp::lib::make_shared<websocketpp::lib::asio::ssl::context>(
            websocketpp::lib::asio::ssl::context::sslv23);
    ctx->set_verify_mode(websocketpp::lib::asio::ssl::verify_none);
    return ctx;
}

void AlpacaFeed::run() {
    running_ = true;

    // Alpaca paper trading WebSocket endpoint
    std::string uri = "wss://stream.data.alpaca.markets/v2/iex";

    websocketpp::lib::error_code ec;
    auto con = client_.get_connection(uri, ec);
    if (ec) {
        std::cerr << "[ALPACA] Connection error: " << ec.message() << "\n";
        return;
    }

    hdl_ = con->get_handle();
    client_.connect(con);
    client_.run();   // blocks until connection closes
    queue_.close();
}

void AlpacaFeed::stop() {
    running_ = false;
    websocketpp::lib::error_code ec;
    client_.close(hdl_, websocketpp::close::status::normal, "shutdown", ec);
}

double AlpacaFeed::getPrice() const {
    return lastPrice_;
}

void AlpacaFeed::onOpen(websocketpp::connection_hdl hdl) {
    hdl_ = hdl;
    std::cout << "[ALPACA] Connected\n";
    authenticate();
}

void AlpacaFeed::authenticate() {
    std::string msg = R"({"action":"auth","key":")" + apiKey_ +
                      R"(","secret":")" + apiSecret_ + R"("})";
    client_.send(hdl_, msg, websocketpp::frame::opcode::text);
    std::cout << "[ALPACA] Authenticating...\n";
}

void AlpacaFeed::subscribe() {
    std::string msg = R"({"action":"subscribe","trades":[")" + symbol_ + R"("]})";
    client_.send(hdl_, msg, websocketpp::frame::opcode::text);
    std::cout << "[ALPACA] Subscribed to " << symbol_ << " trades\n";
}

void AlpacaFeed::onMessage(websocketpp::connection_hdl,
                            WsClient::message_ptr msg) {
    std::string payload = msg->get_payload();

    // after auth success, subscribe
    if (payload.find("authenticated") != std::string::npos) {
        std::cout << "[ALPACA] Authenticated\n";
        subscribe();
        return;
    }

    // parse trade price
    if (payload.find("\"T\":\"t\"") != std::string::npos) {
        double price = parsePrice(payload);
        if (price > 0.0) {
            lastPrice_ = price;
            std::cout << std::fixed << std::setprecision(2)
                      << "[PRICE] " << symbol_ << " $" << price << "\n";
            logger_.logPrice(price);
            queue_.push(price);
        }
    }
}

void AlpacaFeed::onClose(websocketpp::connection_hdl) {
    std::cout << "[ALPACA] Connection closed\n";
    queue_.close();
}

void AlpacaFeed::onFail(websocketpp::connection_hdl) {
    std::cerr << "[ALPACA] Connection failed\n";
    queue_.close();
}

double AlpacaFeed::parsePrice(const std::string& payload) {
    // extract "p": value from JSON without a full JSON library
    auto pos = payload.find("\"p\":");
    if (pos == std::string::npos) return 0.0;
    pos += 4;
    auto end = payload.find_first_of(",}", pos);
    if (end == std::string::npos) return 0.0;
    try {
        return std::stod(payload.substr(pos, end - pos));
    } catch (...) {
        return 0.0;
    }
}