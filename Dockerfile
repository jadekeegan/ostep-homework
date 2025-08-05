# Use a lightweight Ubuntu image
FROM ubuntu:22.04

# Set environment variables to avoid interactive prompts during install
ENV DEBIAN_FRONTEND=noninteractive

# Update and install essential tools: Python, GCC, Make, etc.
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    gcc \
    g++ \
    make \
    build-essential \
    vim \
    git \
    curl \
    && apt-get clean

# Set working directory inside container
WORKDIR /workspace

# Copy current directory contents into container
COPY . .

# Default command: drop into a bash shell
CMD ["/bin/bash"]
