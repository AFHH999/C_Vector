CC = clang
CFLAGS = -Wall -Wextra -pedantic -std=c11 -Iinc -MMD -MP

# Target names
TARGET = bin/vector
TEST_TARGET = bin/test_vector

# Auto discover components, except main.c
ENGINE_SRCS = $(filter-out src/main.c, $(wildcard src/*.c))
ENGINE_OBJS = $(patsubst src/%.c, bin/%.o, $(ENGINE_SRCS))

all: $(TARGET)

# The executable production
$(TARGET): bin/main.o $(ENGINE_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) bin/main.o $(ENGINE_OBJS)

# The test executable
test: $(TEST_TARGET)
	./$(TEST_TARGET)

TEST_SRCS = $(wildcard tests/*.c)
TEST_OBJS = $(patsubst tests/%.c, bin/%.o, $(TEST_SRCS))

DEPS = $(ENGINE_OBJS:.o=d) bin/main.d $(TEST_OBJS:.o=d)

$(TEST_TARGET): $(TEST_OBJS) $(ENGINE_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJS) $(ENGINE_OBJS) -lcriterion

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: tests/%.c
	$(CC) $(CFLAGS) -c $< -o $@

bin:
	mkdir -p bin

clean:
	rm -f bin/*.o bin/*.d $(TARGET) $(TEST_TARGET)

memcheck:
	valgrind ./bin/test_vector

-include $(DEPS)

.PHONY: all test clean
