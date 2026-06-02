# Market Simulator

A real-time market simulator written in C++. Asset prices tick at controlled intervals, orders flow through a matching engine, and a trading algorithm manages a portfolio autonomously — all running concurrently across multiple threads.

Built to mirror the architecture of real trading infrastructure: low-latency, multi-threaded, and modular.

---

## Features

- **Live price engine** — generates asset prices via random walk or CSV replay
- **Order book** — matches buy/sell orders by bid/ask price
- **Portfolio tracker** — tracks cash, holdings, and P&L in real time
- **Trading algorithm** — automated strategy (moving average crossover)
- **Logger** — writes all trades and price ticks to file
- **Multi-threaded** — price, matching, and strategy run on separate threads

---

## Project Structure

```
market-sim/
├── include/        # Header files
├── src/            # Source files
├── tests/          # Unit tests
├── docs/           # Software Design Document
├── CMakeLists.txt
├── Dockerfile
└── .gitlab-ci.yml
```

---

## Build & Run

**Prerequisites:** C++17, CMake 3.15+, Linux

```bash
mkdir build && cd build
cmake ..
make
./market-sim
```

**With Docker:**
```bash
docker build -t market-sim .
docker run market-sim
```

---

## Roadmap

- [ ] Multiple simultaneous assets
- [ ] CSV replay mode using historical data
- [ ] Terminal UI with ncurses
- [ ] Additional strategies (mean reversion, momentum)