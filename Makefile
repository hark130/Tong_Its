CC      = g++
CFLAGS  = -std=c++14 -I include
OUT		= game.exe
RM      = rm -f

all: 
	$(CC) $(CFLAGS) -o $(OUT) source/Clear_Screen.cpp source/Tong_Its.cpp source/Tong_Its_Playing_Card.cpp source/Tong_Its_Game.cpp source/Tong_Its_Player.cpp source/Tong_Its_AI_Player.cpp main.cpp -lncurses

clean:
	$(RM) *.o *.i *.exe $(OUT)

# g++ --std=c++14 -I include source/Clear_Screen.cpp source/Tong_Its.cpp main.cpp -o game.exe -lncurses