#pragma once
#include <string>
#include <atomic>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include "PriceQueue.hpp"
#include "Logger.hpp"

typedef websocketpp::client<websocketpp::config::asio_tls_client> WsClient;
typedef websocketpp:: lib::shared_ptr<websocketpp::lib::asio::ssl::context> SslContext;

class AlpacaFeed {
public:
    AlpacaFeed(const std::string& apiKey,
               const std::string& apiSecret,
               const std::string& symbol,
               PriceQueue& queue,
               Logger& logger);

    void run();    // blocks — call on its own thread
    void stop();
    double getPrice() const;

private:
        std::string apiKey_;
        std::string apiSecret_;
        std::string symbol_;
        PriceQueue& queue_;
        Logger& logger_;
        std::atomic<bool> running_;
        double lastPrice_ = 0.0;
        WsClient client_;
        websocketpp::connection_hdl hdl_;
        void onOpen(websocketpp::connection_hdl hdl);
        void onMessage(websocketpp::connection_hdl hdl, WsClient::message_ptr msg);
        void onClose(websocketpp::connection_hdl hdl);

        void onFail(websocketpp::connection_hdl hdl);
        SslContext onTlsInit(websocketpp::connection_hdl hdl);
        void authenticate();
        void subscribe();
        double parsePrice(const std:: string& payload);
};