*This project has been created as part of the 42 curriculum by helfayez.*

# Philosophers

## Description

Philosophers is a classic 42 School project that simulates the **Dining Philosophers**
problem, a well-known thread synchronization exercise. A number of philosophers sit
around a round table, with one fork placed between each pair of neighbors. Each
philosopher repeatedly cycles through three states:

1. **Thinking**
2. **Eating** — requires picking up both the left and right fork at the same time
3. **Sleeping**

If a philosopher goes longer than `time_to_die` milliseconds without starting to eat,
they die and the simulation stops immediately.

The goal of the project is to implement this simulation using POSIX threads and
mutexes/condition variables, correctly handling concurrent access to shared resources
(the forks) while avoiding data races, deadlocks, and philosophers starving
unnecessarily — without using a busy global lock that would serialize everything.

## Instructions

### Compilation

```bash
make        # builds the philo executable
make clean  # removes object files
make fclean # removes object files and the executable
make re     # fclean then all
```

Compiled with `-Wall -Wextra -Werror -pthread`.

### Execution

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) at the table |
| `time_to_die` | Time in milliseconds after which a philosopher dies if they haven't started eating |
| `time_to_eat` | Time in milliseconds a philosopher spends eating (holding both forks) |
| `time_to_sleep` | Time in milliseconds a philosopher spends sleeping |
| `number_of_times_each_philosopher_must_eat` | Optional. If given, the simulation stops once every philosopher has eaten this many times |

Examples:

```bash
./philo 5 800 200 200
./philo 4 410 200 200 7
```

Each output line has the format:

```
<timestamp in ms since start> <philosopher id> <event: has taken a fork / is eating / is sleeping / is thinking / died>
```

### Project layout

| File | Responsibility |
|---|---|
| `philo.h` | Struct definitions (`t_data`, `t_philo`, `t_fork`) and function prototypes |
| `philo.c` | `main`, data initialization (`init_data`), creating/joining philosopher threads |
| `its_true.c` | Argument parsing/validation (`parse_args`, `check_args`) and the end-of-simulation check (`all_ate_enough`) |
| `fork.c` | Fork locking logic (`fork_lock`/`fork_unlock`), taking both forks (`take_forks`), `craete_fork` |
| `monitor_and_routine.c` | Each philosopher's thread routine (`routine`), the single-philosopher edge case (`one_philo`), and the watchdog thread (`monitor`) |
| `dead_fun_and_eat.c` | Death detection (`check_death`), eating (`eat`), priority-to-eat check (`is_hungriest`) |
| `utils.c` | Helpers: timing (`get_time`), thread-safe printing (`print_status`), `ft_atoi`, `is_valid_number`, `smart_sleep` |

### Technical choices (deadlock / data race avoidance)

- Each philosopher owns a left and right fork (shared with neighbors); each fork's
  `in_use` state is protected by a `table_lock` mutex.
- Instead of a fixed pickup order (which can deadlock), a philosopher checks
  `is_hungriest` to make sure they are the most "starved" of their two neighbors before
  taking both forks together, waiting on `pthread_cond_timedwait` until they're free.
- Releasing a fork (`fork_unlock`) signals the waiting neighbors via
  `pthread_cond_signal`.
- A `dead_lock` mutex protects the shared death flag; a per-philosopher `meal_lock`
  protects the last meal time and meal count.
- A dedicated `monitor` thread continuously checks whether any philosopher has died or
  whether everyone has eaten enough to end the program.
- The single-philosopher case (`num_philo == 1`) is handled separately in `one_philo`,
  since one philosopher only has one fork and can never eat.

## Resources

- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- POSIX Threads Programming (LLNL tutorial) — `pthread_create`, `pthread_mutex_*`, `pthread_cond_*`
- `man` pages: `pthread_create(3)`, `pthread_mutex_lock(3)`, `pthread_cond_timedwait(3)`, `gettimeofday(2)`, `usleep(3)`
- 42 School subject PDF for Philosophers

### AI usage

An AI assistant (Claude) was used to write the project's documentation (this
README.md) by reading and summarizing the existing source code. It was not used to
design the synchronization algorithm or generate the C source files themselves.
