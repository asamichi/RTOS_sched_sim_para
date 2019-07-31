CC			= g++
CFLAGS		= -std=c++11 -fopenmp
  # -std=c**0x  #ex -L/usr/local/lib .cから.oが作られる時のオプション
OPTION 		= -Wall -std=c++11#std=c++0x
DEST 		= ./
LIBS		= #ex    -lhoge -lm
LDFLAGS		= -std=c++11 #-std=c++0x
NAME 		= benchmark.exe
OBJS 		= bench.o
C = CP

#$(NAME):	$(OBJS)
#		$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(NAME)

#.cc.exe:
#		$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
#a.exe: a.cc a.h b.cc

help:;
	@echo ' make run               | make r'
	@echo 'make run_checkpoint     | make rc'
	@echo 'make run_ideal_wcet     | make riw'
	@echo 'make run_first_check    | make rfc'
	@echo 'make clean              | make c'
	@echo 'make all'
	@echo 'make e                  | make easy   :compile only'
	@echo 'make eva                              :make rc riw rfc and mv csvfile to save'
	@echo 'make git                              :git add and commit'
	@echo 'make eva_A                            :run for evaluation
e:;
	make easy
easy:;
		make clean
		make all
all:	$(NAME)
benchmark.exe:	simulater.cc bench.cc bench.h
		$(CC) $(CFLAGS) simulater.cc bench.cc $(LDFLAGS) $(LIBS) -o benchmark.exe
r:;
	make run
run:;
		make e
		@for i in 1 2 3 4 5 6 7 8 9 10 11; do \
		echo $$i;\
			./benchmark.exe -s $$i -c ./output/result -m normal > ./output/story$$i\_normal.log    && \
			./benchmark.exe -s $$i -c ./output/result -m hc > ./output/story$$i\_hc.log            && \
			./benchmark.exe -s $$i -c ./output/result -m no_lock > ./output/story$$i\_no\_lock.log; \
			./benchmark.exe -s $$i -c ./output/result -m lock > ./output/story$$i\_lock.log;          \
		done
		@echo "allocate result"
		./benchmark.exe -a ./output/result
		@echo done
rc:;
	make run_checkpoint
run_checkpoint:;
		make e
		@for j in 10 100 1000 5000 10000; do \
		echo "CHECK_POINT = $$j"; \
		for i in 2 3 4 5 6 7 8 9 10 11; do \
			echo $$i;\
			./benchmark.exe -s $$i -c ./output/result -m normal  CHECK_POINT $$j  > ./output/story$$i\_normal.log    && \
			./benchmark.exe -s $$i -c ./output/result -m hc      CHECK_POINT $$j   > ./output/story$$i\_hc.log            && \
			./benchmark.exe -s $$i -c ./output/result -m no_lock CHECK_POINT $$j   > ./output/story$$i\_no\_lock.log; \
			./benchmark.exe -s $$i -c ./output/result -m lock    CHECK_POINT $$j   > ./output/story$$i\_lock.log;      \
		done\
		done
		@echo "allocate result"
		./benchmark.exe -a ./output/result
		@echo done
riw:;
	make run_ideal_wcet
run_ideal_wcet:;
		make e
		@for i in 2 3 4 5 6 7 8 9 10 11; do \
		echo $$i;\
			./benchmark.exe -s $$i -c ./output/result -m normal -w  > ./output/story$$i\_normal.log    && \
			./benchmark.exe -s $$i -c ./output/result -m hc -w      > ./output/story$$i\_hc.log            && \
			./benchmark.exe -s $$i -c ./output/result -m no_lock -w > ./output/story$$i\_no\_lock.log; \
			./benchmark.exe -s $$i -c ./output/result -m lock -w    > ./output/story$$i\_lock.log;          \
		done
		@echo "allocate result"
		./benchmark.exe -a ./output/result
		@echo done
rfc:;
	make run_first_check
run_first_check:;
		make e
		@for i in 1 2 3 4 5 6 7 8 9 10; do \
		echo $$i;\
			./benchmark.exe -s $$i -c ./output/result -m normal -w > ./output/story$$i\_normal.log;\
			./benchmark.exe -s $$i -c ./output/result -m normal -w non_first_check > ./output/story$$i\_normal\_nfc.log; \
			./benchmark.exe -s $$i -c ./output/result -m no_lock -w > ./output/story$$i\_nolock.log; \
			./benchmark.exe -s $$i -c ./output/result -m no_lock -w non_first_check > ./output/story$$i\_nolock\_nfc.log; \
			./benchmark.exe -s $$i -c ./output/result -m lock -w  > ./output/sotry$$i\_lock.log;   \
			./benchmark.exe -s $$i -c ./output/result -m lock -w  non_first_check > ./output/story$$i\_lock\_nfc.log;   \
		done
		@echo "allocate result"
		./benchmark.exe a 6 ./output/result
		@echo done
test:; make clean
		make e
		./benchmark.exe -s 1 -m normal  -c ./output/result  > ./output/test\_normal.log
		./benchmark.exe -s 1 -m hc      -c ./output/result     > ./output/test\_hc.log
		./benchmark.exe -s 1 -m no_lock -c ./output/result > ./output/test\_no\_lock.log
		./benchmark.exe -s 1 -m lock    -c ./output/result > ./output/test\_lock.log
		./benchmark.exe -s 1 -m nlock    -c ./output/result > ./output/test\_nlock.log
		./benchmark.exe -a ./output/result
c:;		make clean
clean:;	rm -f *.o *~ $(NAME) *.exe ./output/*

pc:;	make pclean
pclean:; make clean
		rm -f story* result*  *.csv*
# 以下評価用に随時作った奴
eva:;
	make run_checkpoint
	mv ./output/result_shaping.csv ./save/result_c.csv
	make run_ideal_wcet
	mv ./output/result_shaping.csv ./save/result_w.csv
	make rfc
	mv ./output/result_shaping.csv ./save/result_fc.csv
git:;
	make c
	git add -A
	git commit -am "$C"
eva2:;
		make e
		@for j in 10 100 1000 5000 10000; do \
		echo "CHECK_POINT = $$j"; \
		for i in 2 3; do \
			echo $$i;\
			./benchmark.exe -s $$i -c ./output/result -m normal  CHECK_POINT $$j  > ./output/story$$i\_normal.log    && \
			./benchmark.exe -s $$i -c ./output/result -m hc      CHECK_POINT $$j   > ./output/story$$i\_hc.log            && \
			./benchmark.exe -s $$i -c ./output/result -m lock    CHECK_POINT $$j   > ./output/story$$i\_lock.log;      \
		done\
		done
		@echo "allocate result"
		./benchmark.exe a 3 ./output/result
		@echo done
		mv ./output/result_shaping.csv ./save/result_c.csv
		make e
		@for i in 2 3; do \
		echo $$i;\
			./benchmark.exe -s $$i -c ./output/result -m normal -w  > ./output/story$$i\_normal.log   &&  \
			./benchmark.exe -s $$i -c ./output/result -m hc -w      > ./output/story$$i\_hc.log       &&  \
			./benchmark.exe -s $$i -c ./output/result -m lock -w    > ./output/story$$i\_lock.log;      \
		done
		@echo "allocate result"
		./benchmark.exe a 3 ./output/result
		@echo done
		mv ./output/result_shaping.csv ./save/result_w.csv
eva3:;
		make e
		@for j in 10 100 1000 5000 10000; do \
		echo "CHECK_POINT = $$j"; \
		for i in 12 13; do \
			echo $$i;\
			./benchmark.exe -s $$i -c ./output/result -m normal  CHECK_POINT $$j  > ./output/story$$i\_normal.log    && \
			./benchmark.exe -s $$i -c ./output/result -m hc      CHECK_POINT $$j   > ./output/story$$i\_hc.log            && \
			./benchmark.exe -s $$i -c ./output/result -m lock    CHECK_POINT $$j   > ./output/story$$i\_lock.log;      \
		done\
		done
		@echo "allocate result"
		./benchmark.exe a 3 ./output/result
		@echo done
		mv ./output/result_shaping.csv ./save/result_c.csv
		make e
		@for i in 12 13; do \
		echo $$i;\
			./benchmark.exe -s $$i -c ./output/result -m normal -w  > ./output/story$$i\_normal.log   &&  \
			./benchmark.exe -s $$i -c ./output/result -m hc -w      > ./output/story$$i\_hc.log       &&  \
			./benchmark.exe -s $$i -c ./output/result -m lock -w    > ./output/story$$i\_lock.log;      \
		done
		@echo "allocate result"
		./benchmark.exe a 3 ./output/result
		@echo done
		mv ./output/result_shaping.csv ./save/result_w.csv
# 回数の指定
NUM = 1000

# 複数回実行用
eva_A:;
		make e
		@for j in 10 100; do \
		echo "CHECK_POINT = $$j"; \
		for i in 1 2 3 4 5 6; do \
			echo $$i;\
			./benchmark.exe -s $$i -c ./output/result -m normal  CHECK_POINT $$j -n $(NUM) > ./output/story$$i\_normal.log    && \
			./benchmark.exe -s $$i -c ./output/result -m hc      CHECK_POINT $$j -n $(NUM)  > ./output/story$$i\_hc.log            && \
			./benchmark.exe -s $$i -c ./output/result -m lock    CHECK_POINT $$j -n $(NUM)  > ./output/story$$i\_lock.log;      \
		done\
		done
		@echo "allocate result"
		./benchmark.exe a 3 ./output/result
		@echo done
		cp -r ./output ./eva
# 未使用。新シナリオでやったやつ。ちゃんと動くよ
eva_game:;
		make e
		@for j in 10 100; do \
		echo "CHECK_POINT = $$j"; \
		for i in 7 8 9 10; do \
			echo $$i;\
			./benchmark.exe -s $$i -c ./output/result -m lock    CHECK_POINT $$j -n $(NUM)  > ./output/story$$i\_lock.log;      \
		done\
		done
		@echo "allocate result"
		./benchmark.exe a 1 ./output/result
		@echo done
		cp -r ./output ./eva4


# 複数回実行用
eva_para:;
		make e
		@for j in 100; do \
		echo "CHECK_POINT = $$j"; \
		for i in 1; do \
			echo $$i;\
			./benchmark.exe -s $$i -c ./output/result -m normal  CHECK_POINT $$j -n $(NUM) > ./output/story$$i\_normal.log    && \
			./benchmark.exe -s $$i -c ./output/result -m hc      CHECK_POINT $$j -n $(NUM)  > ./output/story$$i\_hc.log            && \
			./benchmark.exe -s $$i -c ./output/result -m lock    CHECK_POINT $$j -n $(NUM)  > ./output/story$$i\_lock.log;      \
		done\
		done
		@echo "allocate result"
		./benchmark.exe a 3 ./output/result
		@echo done
		cp -r ./output ./eva