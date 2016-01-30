################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DataHandler.cpp \
../Kinect.cpp \
../KinectCurseur.cpp \
../main.cpp 

OBJS += \
./DataHandler.o \
./Kinect.o \
./KinectCurseur.o \
./main.o 

CPP_DEPS += \
./DataHandler.d \
./Kinect.d \
./KinectCurseur.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


