# FreeRTOS 

A comprehensive collection of FreeRTOS demonstrations on STM32F405RGTX_ microcontrollers. This repository showcases fundamental RTOS concepts including task management, inter-task communication, synchronization mechanisms, and priority handling.

## Overview

This project contains practical implementations of core FreeRTOS features, providing hands-on examples for embedded systems developers learning real-time operating systems. Each example is self-contained and demonstrates specific RTOS concepts with LED-based visual feedback.

## Repository Structure

```
RTOS/
├── Task_creatio/           # Basic task creation and scheduling
├── task_creation_lcd/      # Task creation with LCD display
├── message_queue/          # Queue-based inter-task communication
├── priority_inversion_inheritance/  # Priority inversion demonstration with mutex
├── semaphore/              # Binary semaphore and task synchronization
└── README.md               # This file
```

## Projects Overview

### 1. Task Creation (`Task_creatio`)
**Concepts Demonstrated:**
- Basic FreeRTOS task creation
- Task scheduling with different priorities
- `vTaskDelay()` for non-blocking delays
- LED toggling in independent tasks

**Features:**
- **Task 1 (Priority 1)**: Toggles LED1 every 1 second
- **Task 2 (Priority 2)**: Toggles LED2 every 3 seconds
- Demonstrates preemptive scheduling behavior

**Key Learning Points:**
- When a high-priority task is sleeping (`vTaskDelay`), lower-priority tasks run freely
- `vTaskDelay()` releases CPU to other tasks (non-blocking)
- `HAL_Delay()` blocks CPU entirely (blocking)

---

### 2. Task Creation with LCD (`task_creation_lcd`)
**Concepts Demonstrated:**
- Task creation with LCD integration
- Multi-task coordination
- Real-time display updates

**Features:**
- Multiple tasks with LCD feedback
- Visual demonstration of task execution
- Priority-based scheduling with display output

---

### 3. Message Queue (`message_queue`)
**Concepts Demonstrated:**
- Queue creation and management
- Producer-Consumer pattern
- Inter-task communication
- Queue status monitoring

**Features:**
- **Sender Task 1 (Priority 2)**: Sends data value `222` every 2 seconds
- **Sender Task 2 (Priority 3)**: Sends data value `111` every 2 seconds
- **Receiver Task (Priority 4)**: Receives data every 5 seconds
- Queue capacity: 5 items
- Status tracking for send/receive operations

**Key Concepts:**
```c
// Queue creation
SimpleQueue = xQueueCreate(5, sizeof(int));

// Sending data
xQueueSend(SimpleQueue, &data, portMAX_DELAY);

// Receiving data
xQueueReceive(SimpleQueue, &received_data, portMAX_DELAY);
```

**Use Cases:**
- Data buffering between tasks
- Asynchronous communication
- Decoupling producer and consumer timing

---

### 4. Priority Inversion & Inheritance (`priority_inversion_inheritance`)
**Concepts Demonstrated:**
- Priority inversion problem
- Mutex with priority inheritance
- Critical section protection
- Task synchronization

**Features:**
- **Task 1 (Priority 3, High)**: Protected LED blink sequence with mutex
- **Task 2 (Priority 2, Medium)**: Unprotected LED blink sequence
- **Task 3 (Priority 1, Low)**: Protected LED blink sequence with mutex
- Mutex inheritance prevents priority inversion

**Priority Inversion Scenario:**
Without priority inheritance:
1. Low-priority Task 3 acquires mutex
2. High-priority Task 1 preempts but waits for mutex
3. Medium-priority Task 2 can run, blocking high-priority task
4. This inverts the priority system

With Mutex (Priority Inheritance):
- When Task 1 waits for mutex held by Task 3
- Task 3's priority is temporarily elevated to Task 1's level
- Task 2 cannot preempt Task 3
- Priority order is maintained

**Key Differences:**
```c
// vTaskDelay() - CPU is free for other tasks
vTaskDelay(1000);  

// HAL_Delay() - CPU is blocked, no task switching
HAL_Delay(500);
```

**Execution Counters:**
- `count1`, `count2`, `count3` track task execution iterations

---

### 5. Semaphore (`semaphore`)
**Concepts Demonstrated:**
- Binary semaphore creation
- Task synchronization
- Manager-Employee pattern
- Queue and semaphore coordination

**Features:**
- **Manager Task (Priority 3)**: Generates random work tickets
- **Employee Task (Priority 2)**: Processes work tickets
- Binary semaphore signals work availability
- Queue holds work ticket IDs
- Work duration varies based on ticket ID

**Operation Flow:**
```
1. Manager generates random ticket ID (0-255)
2. Manager sends ticket to queue
3. Manager gives semaphore (signals work available)
4. Manager yields CPU to employee
5. Employee takes semaphore (receives signal)
6. Employee receives ticket from queue
7. Employee processes work (delay = ticket ID)
8. Loop continues
```

**Key Functions:**
```c
// Create binary semaphore (deprecated but functional)
vSemaphoreCreateBinary(xWork);

// Take semaphore (non-blocking)
xSemaphoreTake(xWork, 0);

// Give semaphore (signal)
xSemaphoreGive(xWork);

// Voluntary task yield
taskYIELD();
```

**Design Pattern:**
This demonstrates a classic producer-consumer pattern where:
- Manager produces work asynchronously
- Employee consumes work when available
- Semaphore provides signaling mechanism
- Queue provides data transfer mechanism

---

## Hardware Requirements

### Microcontroller
- **STM32F4xx Series** (STM32F401, STM32F407, etc.)
- ARM Cortex-M4 core
- Minimum 128KB Flash, 64KB RAM

### Components
- **LEDs**: Connected to GPIO pins for visual feedback
  - LED1, LED2, LED3 (specific pins defined in CubeMX)
- **16x2 LCD Display** (for lcd examples)
- **Development Board**: STM32 Discovery, Nucleo, or custom board

### Pin Configuration
Pins are configured via STM32CubeMX. Typical assignments:
- LED1: PC6 or user LED1 pin
- LED2: PB7 or user LED2 pin
- LED3: PB6 or user LED3 pin

## Software Requirements

### Development Tools
- **STM32CubeIDE** (v1.10.0 or later)
- **STM32CubeMX** (for configuration)
- **ARM GCC Compiler** (included with CubeIDE)

### FreeRTOS Configuration
- **FreeRTOS Version**: v10.3.1 or later
- **Heap Implementation**: heap_4.c (recommended)
- **Configuration File**: `FreeRTOSConfig.h`

### Key FreeRTOS Settings
```c
#define configUSE_PREEMPTION              1
#define configUSE_IDLE_HOOK               0
#define configUSE_TICK_HOOK               0
#define configCPU_CLOCK_HZ                16000000  // 16 MHz HSI
#define configTICK_RATE_HZ                1000      // 1ms tick
#define configMAX_PRIORITIES              5
#define configMINIMAL_STACK_SIZE          128
#define configTOTAL_HEAP_SIZE             15360     // 15KB
#define configUSE_MUTEXES                 1
#define configUSE_COUNTING_SEMAPHORES     1
#define configUSE_QUEUE_SETS              1
```

## Building and Running

### Using STM32CubeIDE

1. **Import Project**
   ```
   File → Open Projects from File System
   Select project folder (e.g., Task_creatio)
   ```

2. **Configure FreeRTOS** (if not already done)
   ```
   - Open .ioc file
   - Middleware → FREERTOS → Enable
   - Configure tasks, queues, semaphores as needed
   - Generate Code
   ```

3. **Build Project**
   ```
   Project → Build All (Ctrl+B)
   ```

4. **Flash and Debug**
   ```
   Run → Debug (F11)
   ```

### Using Command Line

```bash
# Navigate to project directory
cd Task_creatio

# Build using make
make -j4

# Flash using OpenOCD
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
  -c "program build/Task_creatio.elf verify reset exit"
```

## Understanding FreeRTOS Concepts

### Task States
```
┌─────────────┐
│   Running   │ ← Currently executing
└──────┬──────┘
       │
       ├──→ Ready    ← Ready to run, waiting for CPU
       │
       ├──→ Blocked  ← Waiting for event/delay
       │
       └──→ Suspended ← Manually suspended
```

### Priority Scheduling
- Higher numerical priority = higher importance
- Tasks with equal priority share CPU time (round-robin)
- Higher priority tasks preempt lower priority tasks

### Blocking vs Non-Blocking Delays

| Function | Type | CPU Status | Other Tasks |
|----------|------|------------|-------------|
| `vTaskDelay()` | Non-blocking | Free | Can run |
| `HAL_Delay()` | Blocking | Busy | Cannot run |

### Queue Operation
```c
// FIFO (First In, First Out)
Task A: Send(10) Send(20) Send(30)
                ↓       ↓       ↓
Queue:       [ 10 | 20 | 30 ]
                ↓       ↓       ↓
Task B:   Receive(10) Receive(20) Receive(30)
```

### Mutex vs Binary Semaphore

| Feature | Mutex | Binary Semaphore |
|---------|-------|------------------|
| Purpose | Mutual exclusion | Signaling |
| Owner | Yes | No |
| Priority Inheritance | Yes | No |
| Recursive | Can be | No |
| Best for | Protecting resources | Task synchronization |

## Common RTOS Patterns Demonstrated

### 1. Producer-Consumer (Message Queue)
```c
Producer → [Queue] → Consumer
```
Used for buffering data between different execution rates.

### 2. Manager-Employee (Semaphore)
```c
Manager: Generate Work → Signal → Queue
              ↓
Employee: Wait Signal → Fetch Work → Process
```
Used for asynchronous task coordination.

### 3. Mutual Exclusion (Priority Inversion)
```c
Task A: Take Mutex → Critical Section → Give Mutex
Task B: Take Mutex → Critical Section → Give Mutex
```
Used to protect shared resources from simultaneous access.

## Troubleshooting

### Common Issues

#### 1. Hard Fault on `vTaskStartScheduler()`
**Cause**: Insufficient heap size
**Solution**: Increase `configTOTAL_HEAP_SIZE` in `FreeRTOSConfig.h`

```c
#define configTOTAL_HEAP_SIZE  ((size_t)(20 * 1024))  // 20KB
```

#### 2. Tasks Not Running
**Cause**: Stack overflow
**Solution**: Increase task stack size

```c
xTaskCreate(myTask1, "Task1", 256, NULL, 1, &handle);  // 256 words
```

#### 3. Priority Inversion Still Occurs
**Cause**: Using semaphore instead of mutex
**Solution**: Use `xSemaphoreCreateMutex()` for resource protection

```c
xSemaphore = xSemaphoreCreateMutex();  // Has priority inheritance
```

#### 4. Queue Full/Empty Errors
**Cause**: Producer/consumer rate mismatch
**Solution**: 
- Increase queue size
- Adjust task delays
- Check for blocking time in send/receive

```c
xQueueCreate(10, sizeof(int));  // Increase from 5 to 10
```

#### 5. System Hangs
**Cause**: Deadlock or missing semaphore give
**Solution**: Always pair take/give operations

```c
xSemaphoreTake(mutex, portMAX_DELAY);
// Critical section
xSemaphoreGive(mutex);  // Never forget this!
```

### Debug Tips

1. **Enable Stack Overflow Checking**
```c
#define configCHECK_FOR_STACK_OVERFLOW  2
```

2. **Use FreeRTOS Trace**
```c
#define configUSE_TRACE_FACILITY        1
#define configGENERATE_RUN_TIME_STATS   1
```

3. **Monitor Task States**
```c
eTaskState state = eTaskGetState(taskHandle);
```

4. **Check Heap Usage**
```c
size_t freeHeap = xPortGetFreeHeapSize();
```

## Best Practices

### Task Design
- Keep tasks small and focused on one responsibility
- Use appropriate stack sizes (start with 128-256 words)
- Always use `vTaskDelay()` in infinite loops
- Never use `while(1)` without delays (wastes CPU)

### Priority Assignment
- Assign priorities based on urgency, not importance
- Use fewer priority levels (3-5 is usually sufficient)
- Reserve highest priority for critical real-time tasks

### Queue Usage
- Size queues based on worst-case buffering needs
- Use block time wisely (avoid `portMAX_DELAY` in time-critical code)
- Consider using queue sets for multiple queues

### Synchronization
- Use mutexes for resource protection (has priority inheritance)
- Use binary semaphores for signaling between tasks
- Use counting semaphores for resource counting
- Always give semaphores in reverse order of taking (prevent deadlock)

### Memory Management
- Prefer static allocation for predictable memory usage
- Monitor heap fragmentation with heap_4 or heap_5
- Avoid dynamic memory allocation in ISRs

## Learning Path

### Beginner
1. Start with **Task_creatio** - understand basic scheduling
2. Study **message_queue** - learn inter-task communication

### Intermediate
3. Explore **priority_inversion_inheritance** - understand priority issues
4. Practice **semaphore** - master synchronization patterns

### Advanced
5. Combine concepts in custom projects
6. Add peripherals (UART, I2C, SPI) with RTOS
7. Implement real-world applications

## Future Enhancements

- [ ] Add software timer examples
- [ ] Demonstrate event groups
- [ ] Implement task notifications
- [ ] Add UART communication with queues
- [ ] Create interrupt-safe queue examples
- [ ] Add memory pool demonstrations
- [ ] Implement watchdog integration
- [ ] Add power management examples

## Resources

### Official Documentation
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)
- [STM32 HAL Documentation](https://www.st.com/resource/en/user_manual/dm00105879.pdf)
- [STM32CubeMX User Guide](https://www.st.com/resource/en/user_manual/dm00104712.pdf)

### Books
- "Mastering the FreeRTOS Real Time Kernel" by Richard Barry
- "The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors" by Joseph Yiu

### Tutorials
- [FreeRTOS Tutorial](https://www.freertos.org/tutorial/)
- [STM32 FreeRTOS Guide](https://www.st.com/resource/en/application_note/dm00105262.pdf)

## Contributing

Contributions are welcome! To add new examples:

1. Fork the repository
2. Create a new branch (`git checkout -b feature/new-example`)
3. Add your example with documentation
4. Test thoroughly on hardware
5. Commit changes (`git commit -m 'Add new RTOS example'`)
6. Push to branch (`git push origin feature/new-example`)
7. Open a Pull Request

## License

This project is licensed under the MIT License. FreeRTOS itself is licensed under the MIT License as well.

## Author



## Acknowledgments

- FreeRTOS community for excellent documentation
- STMicroelectronics for comprehensive HAL libraries
- Richard Barry for creating FreeRTOS

---

**Repository Status**: Active Development  
**Last Updated**: December 2025  
**FreeRTOS Version**: 10.3.1+  
**Platform**: STM32F4xx Series

## Quick Start

```bash
# Clone repository
git clone https://github.com/muskum09ari/RTOS.git
cd RTOS

# Open any example in STM32CubeIDE
# Build, flash, and observe LED behavior
```

Happy Learning! 🚀
