# Socket Programming Server

This project is a C++ server-side implementation using Winsock for socket programming. The server listens on port **34570**, processes incoming client data, and sends responses back to the client. The server can handle multiple clients concurrently through multithreading.

## Features

- **Multithreading**: Each client connection is handled by a separate thread for concurrent processing.
- **Flood Fill Algorithm**: Processes image (2D matrice) using a flood fill algorithm.
- **Dynamic Matrix Handling**: Dynamically allocates memory for matrices based on client input.
- **Thresholding**: Applies a threshold to matrix values.
- **Winsock**: Uses Winsock2 for network communication.

## Prerequisites

- **Operating System**: Windows
- **Compiler**: Visual Studio Community Edition (or any C++11 compatible compiler)
- **Libraries**: Winsock2

## Execution criteria
- First clone this in your PC and execute this.
- Then Run Client and you will receive message if connection will succesfully established.
- Server will receive image and after counting number of characters in the image it will send back to client.



## Installation

1. **Clone the Repository:**

   Clone this repository to your local machine:

   ```bash
   git clone https://github.com/MuhammdRehman/your-repository.git

