cmake_minimum_required(VERSION 3.10)
project(first_code)

# Set the C standard you are using (e.g., C99 or C11)
set(CMAKE_C_STANDARD 99)

# Find the ncurses package
find_package(Curses REQUIRED)

# Include the ncurses headers
include_directories(${CURSES_INCLUDE_DIR})

# Add the executable
add_executable(first_code main.c
        world.h
        world.c
        monitor.h
        monitor.c
        house.h
        house.c
        geometry.h
        geometry.c
        worker.h
        worker.c
        limits.h
        semaphore.h
        semaphore.c
        logger.h
        logger.c
)

# Link the executable to the ncurses library
target_link_libraries(first_code ${CURSES_LIBRARIES})
