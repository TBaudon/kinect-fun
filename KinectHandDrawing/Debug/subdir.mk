################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Boutton.cpp \
../DataHandler.cpp \
../Kinect.cpp \
../KinectHandDrawing.cpp \
../main.cpp 

OBJS += \
./Boutton.o \
./DataHandler.o \
./Kinect.o \
./KinectHandDrawing.o \
./main.o 

CPP_DEPS += \
./Boutton.d \
./DataHandler.d \
./Kinect.d \
./KinectHandDrawing.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


