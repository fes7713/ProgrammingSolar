.PHONY: clean All

All:
	@echo "----------Building project:[ ProgrammingRehat - Debug ]----------"
	@cd "ProgrammingRehat" && "$(MAKE)" -f  "ProgrammingRehat.mk"
clean:
	@echo "----------Cleaning project:[ ProgrammingRehat - Debug ]----------"
	@cd "ProgrammingRehat" && "$(MAKE)" -f  "ProgrammingRehat.mk" clean
