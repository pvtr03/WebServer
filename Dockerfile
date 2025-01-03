# Use an official lightweight image
FROM ubuntu:20.04

# Set non-interactive mode to avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    tzdata && \
    ln -fs /usr/share/zoneinfo/Etc/UTC /etc/localtime && \
    dpkg-reconfigure -f noninteractive tzdata

# Set the working directory
WORKDIR /app

# Copy project files
COPY . .

# Build the project
RUN cmake . && make

# Expose the port your web service will run on
EXPOSE 8080

# Command to run your application
CMD ["./WebServer"]