################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/interpolator.cpp \
../src/io.cpp \
../src/io_correlator.cpp \
../src/io_sample.cpp \
../src/random.cpp 

OBJS += \
./src/interpolator.o \
./src/io.o \
./src/io_correlator.o \
./src/io_sample.o \
./src/random.o 

CPP_DEPS += \
./src/interpolator.d \
./src/io.d \
./src/io_correlator.d \
./src/io_sample.d \
./src/random.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/thibaut/workspace/LQCDAnalysis/include" -I"/home/thibaut/workspace/LQCDAnalysis" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


