# About
Computer club events handler. There are seperated logic of i/o operations with events (utils/*, main.cpp) and events themselves.

ComputerClub class has method `handle_event` that is used to validate and handle event

Almost all checks are related to task, some checks are provided other that


# Build
The project requires support of ```C++20``` standart.


```cmd
cd build
cmake ..
make
```
Add test:
```cmd
cmake .. -DBUILD_TESTING=ON
```
By default, it's ```OFF```.

Run program:
```cmd
./computer_club_task <file_name>
```
Run tests:
```cmd
./computer_club_task_tests
```


