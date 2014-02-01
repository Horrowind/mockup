.PHONY: clean All

All:
	@echo "----------Building project:[ smwlib - Debug ]----------"
	@cd "smwlib" && $(MAKE) -f  "smwlib.mk"
clean:
	@echo "----------Cleaning project:[ smwlib - Debug ]----------"
	@cd "smwlib" && $(MAKE) -f  "smwlib.mk" clean
