# Computer Networks Lab Assignments

A collection of Computer Networks laboratory experiments implemented in C, covering IP addressing, subnetting, routing algorithms, error detection and correction, framing techniques, flow control, sliding-window protocols, UDP socket programming, and spread-spectrum communication.

## Table of Contents

1. [Project Overview](#project-overview)
2. [Environment Setup](#environment-setup)
3. [General Rules for Execution](#general-rules-for-execution)
4. [IP Addressing and Routing](#1-ip-addressing-and-routing)
5. [CRC Error Detection](#2-crc-error-detection)
6. [Hamming Code](#3-hamming-code)
7. [Framing Techniques](#4-framing-techniques)
8. [Stop-and-Wait Protocols](#5-stop-and-wait-protocols)
9. [Go-Back-N Protocol](#6-go-back-n-protocol)
10. [Selective Repeat Protocol](#7-selective-repeat-protocol)
11. [Sliding Window Protocol](#8-sliding-window-protocol)
12. [UDP Client-Server Communication](#9-udp-client-server-communication)
13. [Calculator Using UDP](#10-calculator-using-udp)
14. [DSSS Communication](#11-direct-sequence-spread-spectrum-dsss)
15. [FHSS Communication](#12-frequency-hopping-spread-spectrum-fhss)
16. [File Reference](#file-reference)
17. [Troubleshooting](#troubleshooting)

---

## Project Overview

This repository contains the following experiments.

| Directory | Experiment |
|---|---|
| `CN_network/` | Classful IP addressing, subnetting, FLSM, VLSM, and routing |
| `CRC/` | Cyclic Redundancy Check |
| `Hamming/` | Hamming code generation and error detection |
| `bitStuff/` | Bit stuffing |
| `byteCount/` | Byte-count framing |
| `byteStuff/` | Byte stuffing |
| `swNoiceless/` | Stop-and-Wait protocol without simulated loss |
| `swNoisy/` | Stop-and-Wait protocol with simulated acknowledgement loss |
| `GBN/` | Go-Back-N ARQ |
| `SR/` | Selective Repeat ARQ |
| `slidWin/` | Sliding-window communication and timing calculations |
| `UDP/` | Basic UDP client-server communication |
| `Calculator/` | Arithmetic calculator using UDP |
| `DSSS/` | Direct Sequence Spread Spectrum |
| `FHSS/` | Frequency Hopping Spread Spectrum |

## Environment Setup

These programs use C and, for the networking experiments, POSIX socket APIs. The simplest environment for the current source code is **Ubuntu under WSL**.

### 1. Install GCC

Open your WSL terminal and run:

```bash
sudo apt update
sudo apt install build-essential
```

Check the compiler:

```bash
gcc --version
```

### 2. Open the repository

If the repository is on your Windows Desktop under OneDrive, navigate to it using the corresponding WSL path. For example:

```bash
cd /mnt/c/Users/ADMIN/OneDrive/Desktop/CN-Lab-Assignments-main
```

Replace the path with your actual extracted folder location.

Check the contents:

```bash
ls
```

### 3. Compile and execute a C program

General syntax:

```bash
gcc filename.c -o program
./program
```

For programs that use mathematical functions such as `log2()`, `pow()`, or `floor()`, compile with the math library:

```bash
gcc filename.c -o program -lm
```

### 4. Run UDP client-server experiments

Most networking experiments require two terminals.

- **Terminal 1:** Start the receiver or server.
- **Terminal 2:** Start the sender or client.

Start the receiver first because it needs to bind to its listening port before the sender transmits data.

Example:

Terminal 1:

```bash
cd UDP
gcc UDP_Server.c -o serverUDP
./serverUDP
```

Terminal 2:

```bash
cd UDP
gcc UDP_Client.c -o clientUDP
./clientUDP
```

If both terminals are already in the same directory, use `./serverUDP` and `./clientUDP` without changing directories.

**Important:** Run only one receiver using a particular port at a time. Many programs in this repository use port `9000`, while CRC and Hamming use port `8080`. A port already in use can cause a `Bind failed` error.

---

## General Rules for Execution

1. Enter numbers only when the program asks for numbers.
2. Enter binary strings using only `0` and `1` where binary input is expected.
3. Use positive transmission times unless the program explicitly permits zero.
4. For socket programs, keep the receiver terminal open while the sender is running.
5. Use `Ctrl + C` to stop a receiver that waits indefinitely for packets.
6. Use `gcc` to recompile source files after changing them. Existing executables do not automatically update.
7. When a program uses `scanf("%s", ...)`, enter a single word without spaces unless the code specifically supports spaces.

---

# 1. IP Addressing and Routing

Directory: `CN_network/`

This directory contains programs for IP address classification, subnetting, and routing algorithms.

## 1.1 Classful IP Addressing

**Source file:** `classful_add.c`

### Purpose

This program accepts IPv4 addresses and displays information related to classful addressing, such as the IP address class, network and host portions, and related addressing details.

### File description

- `classful_add.c` — C source code for the classful IP addressing experiment.
- `classful_add` — Precompiled executable supplied in the repository.

### Compile and run

```bash
cd CN_network
gcc classful_add.c -o classful_add
./classful_add
```

### Input

The program asks for the number of IP addresses and then asks you to enter the addresses.

Example:

```text
Enter number of IP addresses: 3
192.16.0.254
10.0.0.1
172.16.5.10
```

Enter each IP address in dotted-decimal notation.

### Expected output

The output should identify the addressing class and show the network/host information calculated by the program for each address.

For example, `192.16.0.254` is a Class C address under traditional classful addressing.

**Input precautions**

- Use valid IPv4 addresses.
- Each octet must be between `0` and `255`.
- The program limits the number of input addresses to its configured maximum.
- Do not enter a CIDR prefix such as `/24` unless explicitly requested.

## 1.2 Subnetting, FLSM and VLSM

**Source file:** `subnetting.c`

### Purpose

This program provides a menu-driven implementation of subnetting operations. Depending on the selected option, it calculates subnet information using the network address and CIDR prefix, creates equal-sized subnets using FLSM, or allocates differently sized subnets using VLSM.

### File description

- `subnetting.c` — Source code for the subnetting experiment.
- `subnetting` — Precompiled executable.

### Compile and run

```bash
cd CN_network
gcc subnetting.c -o subnetting
./subnetting
```

### Option A: Basic subnetting

Enter the menu option corresponding to the basic subnetting calculation.

Example inputs:

```text
Enter your choice: 1
Enter Network IP Address: 192.168.1.0
Enter CIDR Prefix (e.g. 24 for /24): 24
```

Use the actual menu option displayed by your program if it differs.

The output should display the subnet information derived from the supplied IP address and prefix.

### Option B: Fixed Length Subnet Masking (FLSM)

FLSM divides a network into subnets of equal size.

Example:

```text
Enter Number of Subnets: 4
```

Use the network address and prefix requested by the program, then enter `4` for the number of required subnets.

The program calculates an equal subnet size that can accommodate all requested subnets.

### Option C: Variable Length Subnet Masking (VLSM)

VLSM allocates subnet sizes according to the number of hosts required by each subnet.

Example:

```text
Enter Number of Subnets: 3
Enter Required Hosts for Subnet 1: 100
Enter Required Hosts for Subnet 2: 50
Enter Required Hosts for Subnet 3: 20
```

Supply the network address and CIDR prefix requested by the program before entering these values.

The program should allocate larger address blocks to the larger host requirements first, subject to the available network address space.

**Input precautions**

- Use a valid network address and prefix.
- The current implementation accepts CIDR prefixes from `0` through `30`.
- For VLSM, enter positive host requirements.
- Ensure the requested subnet blocks fit within the supplied network.
- For ordinary IPv4 subnets, remember that two addresses are normally reserved for the network and broadcast addresses.

## 1.3 Routing Algorithms

**Source file:** `routing.c`

### Purpose

This program implements two routing algorithms:

1. Distance Vector Routing.
2. Link State Routing using Dijkstra's algorithm.

It accepts router names, weighted links between routers, and a source-destination pair.

### File description

- `routing.c` — Source code for graph construction, routing-table calculation, and shortest-path display.
- `routing` — Precompiled executable.

### Compile and run

```bash
cd CN_network
gcc routing.c -o routing
./routing
```

### Sample input

Use this sample graph to test both algorithms:

```text
Enter Number of Routers (maximum 20): 6

Router 1: a
Router 2: b
Router 3: c
Router 4: d
Router 5: e
Router 6: z

Enter Number of Edges: 9

Edge 1: a b 4
Edge 2: a c 2
Edge 3: b c 1
Edge 4: b d 5
Edge 5: c d 8
Edge 6: c e 10
Edge 7: d e 2
Edge 8: d z 6
Edge 9: e z 3
```

Each edge follows the format:

```text
Router1 Router2 Cost
```

The graph is undirected, meaning a link between `a` and `b` can be traversed in both directions.

### Distance Vector Routing

Select option `1` when the program asks for the algorithm.

```text
Enter your choice: 1
Enter Source Router: a
Enter Destination Router: z
```

The output should display routing tables and the shortest path from `a` to `z`.

For the sample graph, the minimum cost is `13`, and one shortest path is:

```text
a -> c -> b -> d -> e -> z
```

### Link State Routing

Run the program again with the same graph and select option `2`.

```text
Enter your choice: 2
Enter Source Router: a
Enter Destination Router: z
```

The program displays the Dijkstra iterations, tentative distances, shortest path, and total cost.

**Input precautions**

- Enter unique router names.
- Use the same router names when entering edges and selecting source and destination.
- Enter non-negative link costs.
- Do not exceed the maximum number of routers.
- Enter a valid source and destination that exist in the graph.

---

# 2. CRC Error Detection

Directory: `CRC/`

## 2.1 CRC Sender

**Source file:** `CRC_Sender.c`

### Purpose

The sender appends zeros to the input data, performs modulo-2 division using the generator key, calculates the CRC remainder, appends the remainder to the original data, and sends the resulting codeword to the receiver through UDP.

### File description

- `CRC_Sender.c` — Calculates the CRC remainder and transmits the codeword.
- `CRC_Receiver.c` — Receives the codeword and checks it using the supplied generator key.
- `transmitter` — Precompiled sender executable.
- `receiver` — Precompiled receiver executable.

### Compile

```bash
cd CRC
gcc CRC_Sender.c -o transmitter
gcc CRC_Receiver.c -o receiver
```

### Run

Terminal 1:

```bash
./receiver
```

Terminal 2:

```bash
./transmitter
```

### Sample input

```text
Enter Data: 1101011011
Enter Key: 10011
```

The data and key must be binary strings.

### Expected output

The sender displays:

- Data after appending zeros.
- Intermediate XOR operations.
- CRC remainder.
- Final transmitted codeword.

The receiver should display the received data and its CRC verification result.

The codeword is constructed as:

`Original data + CRC remainder`

**Input precautions**

- Enter only `0` and `1`.
- Use a generator key containing at least two bits for a meaningful CRC calculation.
- Use the same key at both ends; the sender includes it in the transmitted packet.
- Start the receiver before the sender.

---

# 3. Hamming Code

Directory: `Hamming/`

## 3.1 Hamming Sender

**Source file:** `Hamming_Sender.c`

### Purpose

This program generates a Hamming code using even parity. It inserts parity bits at positions that are powers of two, calculates the parity values, and optionally flips a bit to simulate a transmission error.

### File description

- `Hamming_Sender.c` — Generates the Hamming code, optionally introduces a bit error, and sends the result.
- `Hamming_Receiver.c` — Receives the code and calculates the syndrome to detect an error.
- `Hamming_Sender`, `transmitter` — Precompiled sender executables.
- `Hamming_Receiver`, `receiver` — Precompiled receiver executables.

The duplicate executable names are supplied files; you do not need to run both.

### Compile

```bash
cd Hamming
gcc Hamming_Sender.c -o Hamming_Sender
gcc Hamming_Receiver.c -o Hamming_Receiver
```

### Run

Terminal 1:

```bash
./Hamming_Receiver
```

Terminal 2:

```bash
./Hamming_Sender
```

### Sample input

```text
Enter binary data: 1011
Enter bit position to alter (0 for no error): 0
```

The sender prints the original data, parity-bit positions, generated Hamming code, and final codeword.

To test error detection, run the sender again and enter a valid bit position instead of `0`.

For example:

```text
Enter binary data: 1011
Enter bit position to alter (0 for no error): 3
```

Here, position `3` is the position of the bit to flip in the generated Hamming code, not the third bit of the original data.

### Expected output

- The sender displays the generated Hamming code.
- If a bit is flipped, the sender displays the altered code.
- The receiver should identify the erroneous bit position using the syndrome calculation, where supported by the received code.

**Input precautions**

- Enter only binary data.
- Enter `0` for no error.
- Otherwise, use a position from `1` through the generated code length.
- Start the receiver before sending the code.
- A standard Hamming code detects and corrects a single-bit error under its normal assumptions; it does not reliably correct arbitrary multiple-bit errors.

---

# 4. Framing Techniques

These experiments demonstrate how data frames are separated and how special patterns are handled during transmission.

## 4.1 Bit Stuffing

Directory: `bitStuff/`

### Files

- `BitStuff_Sender.c` — Inserts a `0` after every sequence of five consecutive `1` bits in the data.
- `BitStuff_Receiver.c` — Receives the stuffed bit stream and removes the inserted bits.
- `transmitter`, `receiver` — Precompiled executables.

### Compile

```bash
cd bitStuff
gcc BitStuff_Sender.c -o transmitter
gcc BitStuff_Receiver.c -o receiver
```

### Run

Start the receiver first:

```bash
./receiver
```

Then start the sender in another terminal:

```bash
./transmitter
```

### Sample input

```text
Enter data: 1111101111110
```

### Expected output

The sender displays the original and stuffed bit streams. The receiver displays the received stream and the result of destuffing.

### Input precautions

- Enter only `0` and `1`.
- Include a sequence of five consecutive `1` bits to test the stuffing operation.
- The receiver and sender use UDP port `9000`.

## 4.2 Byte Count Framing

Directory: `byteCount/`

### Files

- `ByteCount_Sender.c` — Accepts multiple frames and prefixes each frame with a count indicating its length.
- `ByteCount_Receiver.c` — Receives the encoded stream and attempts to recover the frames.
- `byteCount_Trans`, `byteCount_Recv`, `transmitter`, `receiver` — Precompiled executables.

### Compile

```bash
cd byteCount
gcc ByteCount_Sender.c -o transmitter
gcc ByteCount_Receiver.c -o receiver
```

### Run

Terminal 1:

```bash
./receiver
```

Terminal 2:

```bash
./transmitter
```

### Sample input

```text
Enter no. of frames: 3
Frame 1: HELLO
Frame 2: NETWORK
Frame 3: VIT
```

For the actual prompts, enter each frame's text when the program asks for it.

### Expected output

The sender displays the original frames and the encoded stream containing the byte-count information. The receiver displays the received stream and the frames it recovers.

**Input precautions**

- Enter a positive number of frames within the array limit.
- Enter one word per frame.
- Avoid empty frames and unusually long strings.
- The byte-count framing implementation depends on its particular encoding format; inspect the receiver output if the count and recovered frames do not match.

## 4.3 Byte Stuffing

Directory: `byteStuff/`

### Files

- `ByteStuff_Sender.c` — Adds frame flags and escape sequences when reserved patterns appear in the input.
- `ByteStuff_Receiver.c` — Receives the stream, identifies frame boundaries, and removes escape sequences.
- `transmitter`, `receiver` — Precompiled executables.

### Compile

```bash
cd byteStuff
gcc ByteStuff_Sender.c -o transmitter
gcc ByteStuff_Receiver.c -o receiver
```

### Run

Terminal 1:

```bash
./receiver
```

Terminal 2:

```bash
./transmitter
```

### Sample input

```text
Enter no. of frames: 2
Enter data: HELLO
Enter data: NETWORK
```

The code uses the flag `01111110` and escape pattern `00011011`.

### Expected output

The sender displays the original frames and stuffed stream. The receiver attempts to identify the frame flags and remove escape sequences.

**Input precautions**

- Enter one word per frame.
- The implementation scans for special bit patterns in the input string, so use binary strings when you want to test those patterns explicitly.
- Use a modest number of short frames to avoid exceeding fixed buffers.
- Start the receiver first.

---

# 5. Stop-and-Wait Protocols

## 5.1 Stop-and-Wait Without Simulated Loss

Directory: `swNoiceless/`

### Files

- `StopAndWait_Noiseless_Sender.c` — Sends packets sequentially and waits for acknowledgements.
- `StopAndWait_Noiseless_Receiver.c` — Receives packets, simulates propagation and transmission delays, and returns acknowledgements.
- `sender`, `transmitter`, `receiver` — Precompiled executables.

### Compile

```bash
cd swNoiceless
gcc StopAndWait_Noiseless_Sender.c -o sender
gcc StopAndWait_Noiseless_Receiver.c -o receiver
```

### Run

Terminal 1:

```bash
./receiver
```

Terminal 2:

```bash
./sender
```

### Sample input

```text
Enter number of packets to transmit: 5
Enter Transmission Time: 1
Enter Propagation Time: 1
```

### Meaning of inputs

- **Number of packets:** Total packets to transmit.
- **Transmission time (`Tt`):** Simulated time required to transmit a packet, in seconds.
- **Propagation time (`Tp`):** Simulated one-way propagation delay, in seconds.

### Expected output

The sender and receiver print packet and ACK events with timestamps. The sender reports the total elapsed time when transmission completes.

**Input precautions**

- Enter a positive packet count.
- Use a positive transmission time.
- Use a non-negative propagation time.
- Keep the receiver running until all packets have been acknowledged.

The displayed timestamps depend on actual process scheduling and integer-second sleeps, so they may differ slightly between runs.

## 5.2 Stop-and-Wait With Simulated ACK Loss

Directory: `swNoisy/`

### Files

- `StopAndWait_Noisy_Sender.c` — Sends packets and waits for acknowledgements, with timeout and retransmission logic intended for a lossy channel.
- `StopAndWait_Noisy_Receiver.c` — Simulates acknowledgement loss based on the supplied loss interval.
- `sender`, `transmitter`, `receiver` — Precompiled executables.

### Compile

```bash
cd swNoisy
gcc StopAndWait_Noisy_Sender.c -o sender
gcc StopAndWait_Noisy_Receiver.c -o receiver
```

### Run

Terminal 1:

```bash
./receiver
```

Terminal 2:

```bash
./sender
```

### Sample input

```text
Enter number of packets to transmit: 5
Enter Transmission Time (sec): 1
Enter Propagation Time (sec): 1
Enter Loss value (0 = no loss, N = every Nth packet lost after N-1 successes): 3
```

### Meaning of the loss value

- `0` — No ACK loss is simulated.
- `3` — The receiver's current logic drops an ACK after two successful ACK transmissions.
- `2` — The receiver drops an ACK after one successful ACK transmission.

### Expected output

The receiver reports received packets and whether each ACK was sent or dropped. The sender is intended to report missing ACKs, timeout events, and retransmissions.

**Important limitation in the supplied source:** the sender's socket receive-timeout configuration is commented out. Consequently, if the receiver drops an ACK, the sender may block indefinitely in `recvfrom()` instead of reaching the retransmission branch. The sample with loss `0` is the safer initial test. For a reliable loss/retransmission demonstration, the timeout code needs to be enabled and tested.

---

# 6. Go-Back-N Protocol

Directory: `GBN/`

Go-Back-N uses a sliding window. If an expected acknowledgement is not received, the sender retransmits outstanding packets beginning with the oldest unacknowledged packet.

## 6.1 Go-Back-N Sender

**Source file:** `GoBackN_Sender.c`

### Files

- `GoBackN_Sender.c` — Implements a sender with a configurable packet count, window size, timing values, and loss interval.
- `GoBackN_Receiver.c` — Receives packets and simulates ACK loss.
- `GoBackN_Sender2.c` — Alternative sender implementation with a different input and timing model.
- `transmitter`, `transmitter2`, `receiver` — Precompiled executables.

### Compile

```bash
cd GBN
gcc GoBackN_Sender.c -o transmitter
gcc GoBackN_Receiver.c -o receiver
```

### Run

Terminal 1:

```bash
./receiver
```

Terminal 2:

```bash
./transmitter
```

### Sample input

```text
Enter number of packets       : 5
Enter window size             : 3
Enter transmission time (Tt)  : 1
Enter propagation time (Tp)   : 1
Enter loss interval           : 0
```

### Meaning of inputs

- **Number of packets:** Total number of packets.
- **Window size:** Maximum number of packets allowed in the sending window.
- **Transmission time:** Time taken to send a packet.
- **Propagation time:** One-way propagation delay.
- **Loss interval:** Controls simulated loss in the receiver. Use `0` for the initial no-loss test.

### Expected output

The sender displays the current window, packet transmissions, ACK arrivals, timeouts, and retransmissions. At the end, it reports the number of transmissions.

**Input precautions**

- Use a positive packet count not exceeding the source limit of `100`.
- The window size must be positive and no greater than the packet count.
- Transmission time must be positive; propagation time must be non-negative.
- Start the receiver before the sender.

**Note:** The timing and loss simulation in this implementation is simplified. For reliable baseline testing, begin with loss interval `0`.

## 6.2 Alternative Go-Back-N Sender

**Source file:** `GoBackN_Sender2.c`

This is a second sender implementation supplied in the repository. It accepts transmission time, propagation time, and a loss interval; its packet-count and window calculations are handled internally.

Compile it with:

```bash
gcc GoBackN_Sender2.c -o transmitter2
```

Run it with the matching `GoBackN_Receiver.c` in another terminal:

```bash
./receiver
```

```bash
./transmitter2
```

Enter the values requested by the prompts:

```text
Enter Transmission Time (Tt): 1
Enter Propagation Time (Tp): 1
Enter loss interval: 0
```

The exact packet count and window are determined by the source code. Start with a loss interval of `0` to test basic communication before experimenting with loss.

---

# 7. Selective Repeat Protocol

Directory: `SR/`

Selective Repeat allows individual packets to be acknowledged and retransmitted without necessarily retransmitting every packet in the window.

## Files

- `SelectiveRepeat_Sender.c` — Sends packets within a window, records ACKs, and retransmits outstanding packets after timeouts.
- `SelectiveRepeat_Receiver.c` — Receives packets and simulates acknowledgement loss.
- `transmitter`, `receiver` — Precompiled executables.

## Compile

```bash
cd SR
gcc SelectiveRepeat_Sender.c -o transmitter -lm
gcc SelectiveRepeat_Receiver.c -o receiver
```

## Run

Terminal 1:

```bash
./receiver
```

Terminal 2:

```bash
./transmitter
```

## Sample input

```text
Enter Transmission Time: 1
Enter Propagation Time: 1
Enter loss interval: 0
```

### Meaning of inputs

- **Transmission time:** Time required to send a packet.
- **Propagation time:** One-way propagation delay.
- **Loss interval:** Controls the receiver's simulated ACK loss.

The sender calculates the number of packets and window size internally from the timing inputs.

### Expected output

The sender displays packet transmissions, ACK arrivals, timeouts, retransmissions, total time, and total transmissions.

**Input precautions**

- Use a positive transmission time.
- Use a non-negative propagation time.
- Start with loss interval `0`.
- Run only one receiver on port `9000`.

**Implementation note:** The supplied sender and receiver are simplified simulations. The calculated packet count is not entered directly by the user, and timing behaviour may not match a full textbook Selective Repeat implementation in every case.

---

# 8. Sliding Window Protocol

Directory: `slidWin/`

## Files

- `SlidingWindow_Sender.c` — Calculates packet and sequence-number parameters from transmission and propagation times, sends packets, and displays the current window and timing state.
- `SlidingWindow_Receiver.c` — Receives setup information and packets, then sends acknowledgements.
- `SlidingWindow_Sender`, `transmitter` — Precompiled sender executables.
- `SlidingWindow_Receiver`, `receiver` — Precompiled receiver executables.

## Compile

```bash
cd slidWin
gcc SlidingWindow_Sender.c -o transmitter -lm
gcc SlidingWindow_Receiver.c -o receiver
```

## Run

Terminal 1:

```bash
./receiver
```

Terminal 2:

```bash
./transmitter
```

## Sample input

```text
Enter Transmission Time (seconds): 1
Enter Propagation Time (seconds): 2
```

### Expected output

The sender calculates and displays:

- Maximum packet count.
- Number of sequence bits.
- Sequence-number range.
- Window size.
- Current window and ACK count.
- Transmission progress and final statistics.

For `Tt = 1` and `Tp = 2`, the supplied formula gives:

- Maximum packets: `5`
- Sequence bits: `2`
- Sequence-number range: `4`
- Window size: `4`

The exact displayed timing depends on the implementation.

**Input precautions**

- Transmission time must be positive.
- Propagation time must be non-negative.
- Start the receiver before the sender.
- Do not run multiple instances of the receiver on port `9000`.

---

# 9. UDP Client-Server Communication

Directory: `UDP/`

## 9.1 UDP Server

**Source file:** `UDP_Server.c`

### Purpose

Creates a UDP socket, binds it to port `9000`, receives a message from the client, and sends a response to the client's address.

### Files

- `UDP_Server.c` — Server-side source.
- `UDP_Client.c` — Client-side source.
- `serverUDP`, `clientUDP` — Precompiled executables.

### Compile

```bash
cd UDP
gcc UDP_Server.c -o serverUDP
gcc UDP_Client.c -o clientUDP
```

### Run

Terminal 1:

```bash
./serverUDP
```

Terminal 2:

```bash
./clientUDP
```

The client sends a message defined in the source code. The server receives it and responds according to its implementation.

**Note:** These source files use predefined message buffers rather than prompting for a message using `scanf()`.

---

# 10. Calculator Using UDP

Directory: `Calculator/`

## Files

- `Calculator_Server.c` — Receives arithmetic operands and an operator, calculates a result, and sends the result to the client.
- `Calculator_Client.c` — Prompts for two numbers and an arithmetic operator, sends them to the server, and displays the result.
- `Calculator_Server`, `Calculator_Client` — Precompiled executables.

## Compile

```bash
cd Calculator
gcc Calculator_Server.c -o Calculator_Server
gcc Calculator_Client.c -o Calculator_Client
```

## Run

Terminal 1:

```bash
./Calculator_Server
```

Terminal 2:

```bash
./Calculator_Client
```

## Sample input

```text
Enter first number: 10
Enter operator (+ - * /): +
Enter second number: 5
```

Expected result:

```text
Result = 15
```

Try the following additional inputs:

| First number | Operator | Second number | Expected arithmetic result |
|---:|:---:|---:|---:|
| 10 | `+` | 5 | 15 |
| 10 | `-` | 5 | 5 |
| 10 | `*` | 5 | 50 |
| 10 | `/` | 2 | 5 |

After the result, the client asks:

```text
Enter 1 to stop else 0:
```

**Important source-code issue:** The supplied client passes `stop` directly to `sendto()` rather than passing `&stop`. This is incorrect for transmitting an integer and can cause a crash or invalid memory access. The server's stop-message handling must also match the corrected client. Fix this issue before relying on the stop prompt. Also, avoid division by zero.

---

# 11. Direct Sequence Spread Spectrum (DSSS)

Directory: `DSSS/`

## Files

- `DSSS_Transmitter.c` — Converts a hardcoded decimal value to binary, spreads each bit using an 11-chip Barker sequence, and sends the spread data.
- `DSSS_Receiver.c` — Receives the spread sequence.
- `DSSS_Transmitter`, `DSSS_Receiver` — Precompiled executables.

## Compile

```bash
cd DSSS
gcc DSSS_Transmitter.c -o DSSS_Transmitter
gcc DSSS_Receiver.c -o DSSS_Receiver
```

## Run

Terminal 1:

```bash
./DSSS_Receiver
```

Terminal 2:

```bash
./DSSS_Transmitter
```

### Input

No keyboard input is required by the supplied transmitter.

The source code contains:

```c
int data = 45;
```

To experiment with another decimal value, change this constant in `DSSS_Transmitter.c`, recompile, and run the program again.

### Expected output

The transmitter displays:

- Original decimal data.
- Binary representation.
- Spread data generated from the Barker sequence.

The receiver displays the received sequence according to its implementation.

**Input precautions**

- Start the receiver first.
- The transmitter uses UDP port `9000`.
- The transmitter uses a hardcoded value, so typing a number into the terminal will not change the transmitted data.

---

# 12. Frequency Hopping Spread Spectrum (FHSS)

Directory: `FHSS/`

## Files

- `FHSS_Sender.c` — Sends predefined message fragments over a sequence of UDP ports to demonstrate frequency hopping.
- `FHSS_Receiver.c` — Listens on the configured ports and receives the message fragments.
- `freqHop_Transmitter`, `freqHop_Receiver` — Precompiled executables.

## Compile

```bash
cd FHSS
gcc FHSS_Sender.c -o freqHop_Transmitter
gcc FHSS_Receiver.c -o freqHop_Receiver
```

## Run

Terminal 1:

```bash
./freqHop_Receiver
```

Terminal 2:

```bash
./freqHop_Transmitter
```

### Input

No keyboard input is required by the supplied sender.

The source defines the ports:

```text
9000, 9001, 9002, 9003, 9004
```

It also defines a hopping sequence and fixed message fragments.

### Expected output

The sender prints each fragment as it is transmitted. The receiver listens on the configured ports and displays the fragments it receives.

**Input precautions**

- Start the receiver before the sender.
- Do not run other experiments that are already using any of these ports.
- This is a predefined demonstration, not an interactive message-entry program.
- The port sequence represents a software simulation of hopping channels; it does not change the physical radio frequency of your computer.

---

# File Reference

This section lists the source files and their roles.

| Directory | Source file | Role |
|---|---|---|
| `CN_network/` | `classful_add.c` | Classful IP address analysis |
| `CN_network/` | `subnetting.c` | Subnetting, FLSM, and VLSM |
| `CN_network/` | `routing.c` | Distance Vector and Dijkstra routing |
| `CRC/` | `CRC_Sender.c` | CRC calculation and transmission |
| `CRC/` | `CRC_Receiver.c` | CRC reception and verification |
| `Hamming/` | `Hamming_Sender.c` | Hamming code generation and optional error injection |
| `Hamming/` | `Hamming_Receiver.c` | Hamming syndrome calculation |
| `bitStuff/` | `BitStuff_Sender.c` | Bit stuffing |
| `bitStuff/` | `BitStuff_Receiver.c` | Bit destuffing |
| `byteCount/` | `ByteCount_Sender.c` | Byte-count framing |
| `byteCount/` | `ByteCount_Receiver.c` | Byte-count frame reception |
| `byteStuff/` | `ByteStuff_Sender.c` | Byte stuffing |
| `byteStuff/` | `ByteStuff_Receiver.c` | Byte destuffing |
| `swNoiceless/` | `StopAndWait_Noiseless_Sender.c` | Stop-and-Wait sender without simulated loss |
| `swNoiceless/` | `StopAndWait_Noiseless_Receiver.c` | Stop-and-Wait receiver without simulated loss |
| `swNoisy/` | `StopAndWait_Noisy_Sender.c` | Stop-and-Wait sender with timeout/retransmission logic |
| `swNoisy/` | `StopAndWait_Noisy_Receiver.c` | Simulated ACK loss |
| `GBN/` | `GoBackN_Sender.c` | Go-Back-N sender |
| `GBN/` | `GoBackN_Sender2.c` | Alternative Go-Back-N sender |
| `GBN/` | `GoBackN_Receiver.c` | Go-Back-N receiver |
| `SR/` | `SelectiveRepeat_Sender.c` | Selective Repeat sender |
| `SR/` | `SelectiveRepeat_Receiver.c` | Selective Repeat receiver |
| `slidWin/` | `SlidingWindow_Sender.c` | Sliding-window timing and sender |
| `slidWin/` | `SlidingWindow_Receiver.c` | Sliding-window receiver |
| `UDP/` | `UDP_Server.c` | Basic UDP server |
| `UDP/` | `UDP_Client.c` | Basic UDP client |
| `Calculator/` | `Calculator_Server.c` | UDP arithmetic server |
| `Calculator/` | `Calculator_Client.c` | UDP arithmetic client |
| `DSSS/` | `DSSS_Transmitter.c` | DSSS spreading and transmission |
| `DSSS/` | `DSSS_Receiver.c` | DSSS reception |
| `FHSS/` | `FHSS_Sender.c` | FHSS message transmission |
| `FHSS/` | `FHSS_Receiver.c` | FHSS reception |

## Understanding the Executable Files

Files such as `transmitter`, `receiver`, `serverUDP`, `clientUDP`, and the named executables are precompiled programs.

They are not C source files, so you do not edit them directly. Instead, edit the corresponding `.c` file and recompile it.

For example:

```bash
gcc BitStuff_Sender.c -o transmitter
```

This command creates a new executable named `transmitter` from the current contents of `BitStuff_Sender.c`.

---

# Troubleshooting

## 1. `gcc: command not found`

Install the compiler:

```bash
sudo apt update
sudo apt install build-essential
```

## 2. `Address already in use` or `Bind failed`

Another process may already be listening on the same UDP port.

Stop the previous receiver with `Ctrl + C` before running another experiment using that port.

## 3. Sender appears stuck

Check that:

- The receiver was started first.
- The receiver is still running.
- Both programs use the same port.
- The receiver has not terminated due to an error.
- The program is not waiting indefinitely for an ACK that was deliberately dropped.

This last issue is particularly relevant to the supplied noisy Stop-and-Wait implementation.

## 4. `Permission denied`

For a compiled program, use:

```bash
./program
```

If you are trying to execute an existing file that lacks executable permissions, you can run:

```bash
chmod +x program
./program
```

Alternatively, compile the source again using GCC.

## 5. `No such file or directory`

Confirm your current directory:

```bash
pwd
ls
```

Make sure the filename and capitalization match the actual source file.

Linux filenames are case-sensitive. For example, `CRC_Sender.c` and `crc_sender.c` are different names.

## 6. Missing math-library symbols

For a program using `log2()`, `pow()`, or `floor()`, compile with `-lm`:

```bash
gcc routing.c -o routing -lm
```

## 7. Output differs from the example

Timing-based programs use sleep calls and UDP communication. Actual timestamps can vary due to process scheduling.

Also, some supplied programs implement simplified protocol simulations. The examples in this README are intended as starting inputs; exact output formatting and protocol behaviour depend on the source implementation.

## 8. A receiver never exits

Several receiver programs run continuously inside a `while (1)` loop. Stop them using:

```text
Ctrl + C
```

Do this only after you have finished the experiment.

---

## Final Recommendations

For each laboratory experiment:

1. Read the description of the sender and receiver.
2. Compile both source files.
3. Start the receiver first, if the experiment uses UDP.
4. Enter the sample input provided in the relevant section.
5. Verify the output and check the received data.
6. Stop the receiver before moving to another experiment using the same port.

**Important:** The README documents the supplied implementations, including known limitations. The noisy Stop-and-Wait sender and UDP calculator client have source-code issues that should be corrected before claiming those particular scenarios run reliably. All other examples should likewise be validated by compiling and executing the current source files rather than assuming that the precompiled binaries are correct.
