all:
	clang-format -i */*.c */*.h
	$(MAKE) -C cat
	$(MAKE) -C grep

clean:
	clang-format -i */*.c */*.h
	$(MAKE) -C cat clean
	$(MAKE) -C grep clean

memcheck:
	$(MAKE) -C cat memcheck
	$(MAKE) -C grep memcheck

rebuild: clean all

test:
	cd tests &&./s21_cat_test.sh && echo &&./s21_grep_test.sh && cd ..

.PHONY: all clean rebuild test