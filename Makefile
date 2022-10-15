all: project

.PHONY: project clean

project:
	make -C src/

project-clean:
	$(MAKE) clean -C src/

clean : project-clean
