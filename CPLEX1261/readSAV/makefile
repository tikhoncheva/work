################################################################################
# Makefile
################################################################################

RM := rm -rf

LIBS := -lilocplex -lcp -lcplex -lconcert -lm -lpthread 

INC_cplex = -I/opt/CPLEX_Studio1261/cplex/include
INC_concert = -I/opt/CPLEX_Studio1261/concert/include
INC_cpoptimizer = -I/opt/CPLEX_Studio1261/cpoptimizer/include

LIB_cplex = -L/opt/CPLEX_Studio1261/cplex/lib/x86-64_linux/static_pic
LIB_concert = -L/opt/CPLEX_Studio1261/concert/lib/x86-64_linux/static_pic
LIB_cpoptimizer = -L/opt/CPLEX_Studio1261/cpoptimizer/lib/x86-64_linux/static_pic


Target = read_sav
ExTarget = readSAV

CPP_SRCS += \
./$(Target).cpp 

OBJS += \
./$(Target).o 

CPP_DEPS += \
./$(Target).d 

all: clean $(Target).o $(Target)

$(Target).o: $(CPP_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DIL_STD $(INC_cplex) $(INC_concert) $(INC_cpoptimizer) -O0 -g -p -pg -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(Target): $(OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ $(LIB_cplex) $(LIB_concert) $(LIB_cpoptimizer) -p -g -pg -o $(ExTarget) $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(CPP_DEPS)$(EXECUTABLES) $(Target) *.lp
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
