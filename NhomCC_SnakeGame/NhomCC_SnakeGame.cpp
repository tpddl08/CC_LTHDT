//Trò chơi con rắn - Nhóm CC
//Code chạy trong Visual Studio dưới dạng console app
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>


//Xác định điều khiển bằng nút mũi tên
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
using namespace std;

// Lớp cơ sở chứa thông tin trò chơi
class GameInfo {
public:
    //Vector lưu tọa độ của phần thân rắn
    vector<int> snakeBodyX;
    vector<int> snakeBodyY;

    //Trạng thái sống của rắn
    bool isAlive = true;

    //Kích thước sân chơi
    int playFieldWidth = 80;
    int playFieldHeight = 20;

    //Tọa độ mặc định của rắn được đặt ở giữa sân chơi
    int x = playFieldWidth / 2, y = playFieldHeight / 2;

    //Các giá trị dòng, cột, điểm hiện tại, điểm cao nhất, tổng điểm tất cả lượt chơi, số lần chơi
    int row=0, col=0, currentScore = 0, currentBest = 0, totalScore = 0, gamesPlayed = 1;

    //Mảng 2D lưu sân chơi
    char** field;

    //Chuỗi lưu tên người chơi
    string playerName;

    //Hàm khởi tạo của lớp GameInfo
    GameInfo() {
        field = new char* [playFieldHeight];
        for (int i = 0; i < playFieldHeight; i++) {
            field[i] = new char[playFieldWidth];
        }
    }

    //Hàm hủy của lớp GameInfo
    ~GameInfo() {
        for (int i = 0; i < playFieldHeight; i++) {
            delete[] field[i];
        }
        delete[] field;
    }

    //Cập nhật điểm cao nhất
    void updateCurrentBest() {
        if (currentScore > currentBest) {
            currentBest = currentScore;
        }
    }
};

// Lớp giao diện trừu tượng cho các đối tượng trên sân
class GameObject {
public:
    virtual void setOnBoard(char** field, int width, int height) = 0;  //Đặt đối tượng trên sân
    virtual ~GameObject() {} //Hàm hủy của lớp GameObject
};

// Lớp Food quản lý thức ăn
class Food : public GameObject {
private:
    int foodPosX, foodPosY;   //Tọa độ thức ăn
    char symbol;   //Kí tự đại diện cho thức ăn

public:
    //Hàm khởi tạo của lớp Food, lấy kí tự '@' làm thức ăn, tạo tạo độ thức ăn ngẫu nhiên
    Food(int width, int height) : symbol('@') {
        foodPosX = rand() % (width - 4) + 2;
        foodPosY = rand() % (height - 4) + 2;
    }

    //Đặt thức ăn trên sân
    void setOnBoard(char** field, int width, int height) override {
        field[foodPosY][foodPosX] = symbol;
    }

    //Cập nhật vị trí mới cho thức ăn
    void updateFoodPOS(int width, int height, const vector<int>& snakeBodyX, const vector<int>& snakeBodyY) {
        bool isUnique = false;   //Kiểm tra vị trí thức ăn có trùng với rắn không

        //Lặp cho đến khi tìm được vị trí không trùng
        while (!isUnique) {

            //Tạo tọa độ ngẫu nhiên
            foodPosX = rand() % (width - 4) + 2;
            foodPosY = rand() % (height - 4) + 2;

            //Giả định vị trí là duy nhất sau đó sử dụng vòng lặp để kiểm tra vị trí phần thân rắn
            isUnique = true;
            for (int i = 0; i < snakeBodyX.size(); i++) {
                if (foodPosY == snakeBodyY[i] && foodPosX == snakeBodyX[i]) {
                    isUnique = false;      //Vị trí bị trùng thì đánh dấu false và kết thúc kiểm tra vị trí thân rắn
                    break;
                }
            }
        }
    }
    //Các hàm get trả về tọa độ thức ăn, và thức ăn
    int getFoodX() const { return foodPosX; }
    int getFoodY() const { return foodPosY; }
    char getSymbol() const { return symbol; }
};

// Lớp Snake quản lý tương tác của rắn
class Snake : public GameInfo, public GameObject {
public:
    bool ateFood = false;  //Biến kiểm tra rắn có ăn thức ăn hay không

    //Cập nhật thân rắn
    void getSnakeBody() {
        snakeBodyX.insert(snakeBodyX.begin(), x);
        snakeBodyY.insert(snakeBodyY.begin(), y);
        if (!ateFood && snakeBodyX.size() > 1) {
            snakeBodyX.pop_back();
            snakeBodyY.pop_back();
        }
    }

    //Đặt thân rắn về trạng thái ban đầu
    void resetSnakeBody() {
        snakeBodyX.clear();
        snakeBodyY.clear();
        snakeBodyX.push_back(x);
        snakeBodyY.push_back(y);
        ateFood = false;
    }

    //Kiểm tra trạng thái của rắn khi ăn thức ăn và va chạm
    void getSnakeCondition(Food& food) {
        ateFood = false;

        //Nếu đầu rắn trùng với vị trí thức ăn thì cập nhật điểm
        if (food.getFoodX() == x && food.getFoodY() == y) {
            currentScore += 10;
            totalScore += 10;
            ateFood = true;
            updateCurrentBest();
        }

        //Nếu đầu rắn chạm vào hàng rào thì đặt trạng thái chết
        if (x == 0 || x == playFieldWidth - 1 || y == 0 || y == playFieldHeight - 1) {
            isAlive = false;
        }

        //Kiếm tra rắn có ăn thân hay không
        for (int i = 1; i < snakeBodyX.size(); i++) {
            //Nếu vị trí của đầu rắn trùng với vị trí thân rắn thì đặt trạng thái thành chết
            if (snakeBodyX[i] == x && snakeBodyY[i] == y) {
                isAlive = false;
            }
        }
    }

    //Đặt đầu và thân rắn trên sân
    void setOnBoard(char** field, int width, int height) override {
        for (int i = 0; i < snakeBodyX.size(); i++) {
            field[snakeBodyY[i]][snakeBodyX[i]] = (i == 0) ? 'O' : 'o';
        }
    }
};

// Lớp GamePlay quản lý trò chơi
class GamePlay : public Snake {
private:
    Food food;

public:
    enum directions { UP, DOWN, LEFT, RIGHT };   //Định nghĩa các hướng di chuyển của rắn
    directions dir = RIGHT;   //Mặc định rắn đi về hướng phải

    //Hàm khởi tạo của lớp GamePlay
    GamePlay() : food(playFieldWidth, playFieldHeight) {}

    //Đặt đầu và thân rắn trên sân
    void setOnBoard(char** field, int width, int height) override {
        for (int i = 0; i < snakeBodyX.size(); i++) {
            field[snakeBodyY[i]][snakeBodyX[i]] = (i == 0) ? 'O' : 'o';
        }
    }

    //Xử lí hướng đi của rắn từ bàn phím
    void getKeyStroke() {
        if (_kbhit()) {  //Kiểm tra có phím nào được bấm không
            switch (_getch()) {   //Lấy kí tự từ bàn phím và xử lí

                //Trong mỗi trường hợp có kiểm tra xem phím muốn bấm có ngược hướng với hướng đi hiện tại của rắn không
            case 'a': case 'A': case KEY_LEFT:        //Trường hợp rắn đi bên trái
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd': case 'D': case KEY_RIGHT:       //Trường hợp rắn đi bên phải
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'w': case 'W': case KEY_UP:          //Trường hợp rắn đi lên
                if (dir != DOWN) dir = UP;
                break;
            case 's': case 'S': case KEY_DOWN:        //Trường hợp rắn đi xuống
                if (dir != UP) dir = DOWN;
                break;
            }
        }
    }

    //Toán tử nhập
    friend istream& operator >> (istream& in, GamePlay& snake) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  //Biến lấy đầu ra của màn hình
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //Đặt chữ màu xanh lam
        cout << "Chao mung ban den voi tro choi con ran!" << endl;
        cout << "Hay su dung cac phim W,A,S,D hoac cac phim mui ten de choi!" << endl << endl;
        cout << "Nhap ten cua ban: ";
        getline(in, snake.GameInfo::playerName);   //Nhập tên người chơi
        return in;
    }

    //Toán tử xuất
    friend ostream& operator << (ostream& out, const GamePlay& snake) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  //Biến lấy đầu ra của màn hình
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);   //Đặt chữ màu xanh lam
        out << "Diem hien tai cua " << snake.GameInfo::playerName << ": " << snake.GameInfo::currentScore << endl;
        out << "So lan da choi: " << snake.GameInfo::gamesPlayed << endl;
        out << "Diem cao nhat: " << snake.GameInfo::currentBest << endl;
        out << "Tong diem: " << snake.GameInfo::totalScore << endl;
        return out;
    }

    //Vẽ sân chơi qua từng hàng và cột
    void setPlayField() {
        for (row = 0; row < playFieldHeight; row++) {
            for (col = 0; col < playFieldWidth; col++) {
                //Nếu vị trí là hàng đầu, cuối, cột đầu, cuối thì đặt là biên
                if (row == 0 || row == playFieldHeight - 1 || col == 0 || col == playFieldWidth - 1) {
                    field[row][col] = '*';  //Kí tự đại diện cho biên
                }
                else
                    field[row][col] = ' ';  //Không phải biên thì xuất ra kí tự trống
            }
        }


        vector<GameObject*> drawableObjects; // Tạo vector chứa con trỏ tới các đối tượng có thể vẽ
        drawableObjects.push_back(&food); // Thêm con trỏ tới đối tượng Food
        drawableObjects.push_back(this);  // Thêm con trỏ tới đối tượng GamePlay

        //Vòng lặp qua từng đối tượng trong Vector
        for (GameObject* obj : drawableObjects) {
            obj->setOnBoard(field, playFieldWidth, playFieldHeight);   //Gọi phương thức setOnBoard để vẽ đối tượng trên sân
        }


        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);   //Lấy handle của console để điều khiển giao diện.
        COORD pos = { 0, 0 };  //Đặt vị trí con trỏ tại điểm (0,0) 
        SetConsoleCursorPosition(hConsole, pos);   //Di chuyển con trỏ console đến vị trí (0, 0)

        cout << *this;    //Hiển thị chỉ số điểm trong lúc chơi 

        //Vòng lặp in từng hàng
        for (row = 0; row < playFieldHeight; row++) {
            for (col = 0; col < playFieldWidth; col++) {   //In các đối tượng trong hàng
                if (field[row][col] == '*') {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Màu vàng cho hàng rào
                }
                else if (field[row][col] == 'O' || field[row][col] == 'o') {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Xanh lá cho rắn
                }
                else if (field[row][col] == '@') {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // Đỏ cho thức ăn
                }
                cout << field[row][col]; //Vẽ sân chơi
            }
            cout << endl;
        }
    }

    //Cập nhật tọa độ của đầu rắn khi di chuyển theo các hướng
    void getSnakeMovement() {
        if (dir == LEFT) x--;
        else if (dir == RIGHT) x++;
        else if (dir == UP) y--;
        else if (dir == DOWN) y++;
    }

    //Hàm chơi lại khi người chơi muốn chơi lại
    void playAgain() {
        //Đặt lại tọa độ bắt đầu ở giữa sân
        x = playFieldWidth / 2;
        y = playFieldHeight / 2;
        gamesPlayed++;  //Tăng số trận đã chơi
        resetSnakeBody();  //Đặt lại kích thước của rắn
        food.updateFoodPOS(playFieldWidth, playFieldHeight, snakeBodyX, snakeBodyY);  //Cập nhật vị trí mới cho thức ăn
        updateCurrentBest();  //Cập nhật điểm cao nhất
        currentScore = 0;  //Đặt lại điểm số hiện tại
        isAlive = true;   //Đặt lại trạng thái thành sống
        dir = RIGHT;   //Đặt lại hướng di chuyển thành phải
        playGame();    //Gọi hàm bắt đầu chơi
    }

    //Hàm lựa chọn muốn chơi lại hay không
    void choicePlayAgain() {
        //Lấy kí tự từ bàn phím
        char choice = _getch();

        //Chọn c nếu muốn chơi lại
        if (choice == 'c' || choice == 'C') {
            system("cls");   //Làm mới màn hình
            playAgain();   //Gọi hàm chơi lại
        }

        //Chọn k nếu không muốn chơi lại
        else if (choice == 'k' || choice == 'K') {
            system("cls");   //Làm mới màn hình
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);   //Biến lấy handle của console
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);    //Xanh lam cho chữ
            cout << "Thong ke tro choi cua " << GameInfo::playerName << endl;
            cout << "So lan choi: " << GameInfo::gamesPlayed << endl;
            cout << "Diem cao nhat: " << GameInfo::currentBest << endl;
            cout << "Tong diem: " << GameInfo::totalScore << endl;

        }
        else {
            const char* choicePlayAgainChat = "Hay chon lai mot trong hai ki tu tren!";
            int choicePlayAgainChatLength = strlen(choicePlayAgainChat); // Lấy chiều dài chuỗi
            COORD pos;
            // Tính toán vị trí để căn giữa cho dòng thứ ba, đặt dòng này dưới dòng Game Over 4 dòng
            pos.Y = static_cast<SHORT>(playFieldHeight / 2 + 4);
            pos.X = static_cast<SHORT>(playFieldWidth / 2 - choicePlayAgainChatLength / 2);

            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);   //Biến lấy handle của console
            SetConsoleCursorPosition(hConsole, pos);   //Đặt con trỏ đến vị trí mới
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);    //Xanh lam cho chữ
            cout << choicePlayAgainChat << endl;
            choicePlayAgain();   //Gọi hàm để chọn lại
        }
    }
    //Hàm bắt đầu trò chơi
    void playGame() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);   // Lấy handle của console
        CONSOLE_CURSOR_INFO cursorInfo;   // Biến lưu thông tin con trỏ console
        GetConsoleCursorInfo(hConsole, &cursorInfo);   // Lấy thông tin hiện tại của con trỏ console
        cursorInfo.bVisible = FALSE;   //Ẩn con trỏ console
        SetConsoleCursorInfo(hConsole, &cursorInfo);   //Áp dụng thiết lập ẩn con trỏ

        //Vòng lặp khi rắn còn sống
        while (isAlive) {
            setPlayField();   //Vẽ sân chơi lên console
            getKeyStroke();   //Xử lý phím bấm từ người chơi
            getSnakeMovement();   //Cập nhật vị trí đầu rắn
            getSnakeCondition(food);   //Kiểm tra trạng thái rắn
            getSnakeBody();    //Cập nhật thân rắn

            //Nếu rắn ăn thức ăn thì tạo vị trí mới cho thức ăn
            if (ateFood) {
                food.updateFoodPOS(playFieldWidth, playFieldHeight, snakeBodyX, snakeBodyY);
            }

            //Trường hợp rắn chết
            if (!isAlive) {

                //Hiện dòng Game Over
                string gameOverChat = "Game Over, " + playerName + " da ghi duoc " + to_string(currentScore) + " diem!";
                int gameOverChatLength = gameOverChat.length(); // Lấy chiều dài chuỗi

                // Tính toán vị trí để căn giữa cho dòng thứ nhất
                COORD pos;
                pos.Y = static_cast<SHORT>(playFieldHeight / 2); // Đặt ở giữa theo chiều dọc
                pos.X = static_cast<SHORT>(playFieldWidth / 2 - gameOverChatLength / 2); // Căn giữa theo chiều ngang

                SetConsoleCursorPosition(hConsole, pos);   //Đặt con trỏ đến tọa độ ở trên
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);   //Xanh lam cho chữ
                cout << gameOverChat << endl; // In chuỗi đã căn giữa


                //Hiện dòng hỏi chơi lại
                const char* playAgainChat = "Ban co muon choi lai khong? Hay chon Co(c)/Khong(k)";
                int playAgainChatLength = strlen(playAgainChat); // Lấy chiều dài chuỗi

                // Tính toán vị trí để căn giữa cho dòng thứ hai, đặt dòng này dưới dòng Game Over 2 dòng
                pos.Y = static_cast<SHORT>(playFieldHeight / 2 + 2);
                pos.X = static_cast<SHORT>(playFieldWidth / 2 - playAgainChatLength / 2);

                SetConsoleCursorPosition(hConsole, pos);   //Đặt con trỏ đến vị trí mới
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);    //Xanh lam cho chữ
                cout << playAgainChat << endl; // In chuỗi đã căn giữa

                choicePlayAgain();   //Gọi hàm chọn lựa chọn có chơi lại hay không

            }
            Sleep(100);   //Tạo tốc độ cho trò chơi dừng mỗi 100ms sau đó tiếp tục chạy
        }
    }
};

int main() {
    srand(time(NULL));  //Tạo vị trí thức ăn một cách ngẫu nhiên
    GamePlay play;
    cin >> play;   //Nhập tên người chơi
    system("cls");   //Làm mới màn hình
    play.playGame();   //Chạy trò chơi
    return 0;
}