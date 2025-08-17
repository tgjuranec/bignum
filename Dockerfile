FROM debian:bookworm-slim
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libboost-all-dev \
    git \
    && apt-get clean && rm -rf /var/lib/apt/lists/*



