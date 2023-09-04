#include <iostream>
#include <vector>
#include <cmath>
#include <string>
using namespace std;

// Define the dimensions of the 4D chess board 8x8x8x8
const int BOARD_SIZE = 8;

// Class to represent a chess piece
class ChessPiece {
public:
    //two attributes to set (color and type)
    ChessPiece(string p_color, string p_type){
        color = p_color;
        type = p_type;
    }

    virtual ~ChessPiece() {}

    string getColor() const {
        return color;
    }

    string getType() const {
        return type;
    }

    //must implement check to see if the piece jumps over another piece (not allowed unless knight)
    virtual bool isValidMove(int fromX, int fromY, int fromZ, int fromW, int toX, int toY, int toZ, int toW) const = 0;
    
private:
    string color;
    string type;
    
};

class King : public ChessPiece {
public:
    King(string color) : ChessPiece(color, "KING") {}

    bool isValidMove(int fromX, int fromY, int fromZ, int fromW, int toX, int toY, int toZ, int toW) const override {
        int dx = abs(toX - fromX);
        int dy = abs(toY - fromY);
        int dz = abs(toZ - fromZ);
        int dw = abs(toW - fromW);

        return (dx <= 1 && dy <= 1 && dz <= 1 && dw <= 1);
    }
};

class Queen : public ChessPiece {
public:
    Queen(string color) : ChessPiece(color, "QUEEN") {}

    bool isValidMove(int fromX, int fromY, int fromZ, int fromW, int toX, int toY, int toZ, int toW) const override {
        int dx = abs(toX - fromX);
        int dy = abs(toY - fromY);
        int dz = abs(toZ - fromZ);
        int dw = abs(toW - fromW);

        return (dx == dy && dy == dz && dz == dw) ||
               (dx == dy && dy == dz && dz != dw) ||
               (dx == dy && dy != dz && dz == dw) ||
               (dx == dy && dy != dz && dz != dw) ||
               (dx != dy && dy == dz && dz == dw) ||
               (dx != dy && dy == dz && dz != dw) ||
               (dx != dy && dy != dz && dz == dw);
    }
};

class Rook : public ChessPiece {
public:
    Rook(string color) : ChessPiece(color, "ROOK") {}

    bool isValidMove(int fromX, int fromY, int fromZ, int fromW, int toX, int toY, int toZ, int toW) const override {
        int dx = abs(toX - fromX);
        int dy = abs(toY - fromY);
        int dz = abs(toZ - fromZ);
        int dw = abs(toW - fromW);

        return (dx == 0 && dy == 0 && dz == 0 && dw != 0) ||
               (dx == 0 && dy == 0 && dz != 0 && dw == 0) ||
               (dx == 0 && dy != 0 && dz == 0 && dw == 0) ||
               (dx != 0 && dy == 0 && dz == 0 && dw == 0);
    }
};

class Bishop : public ChessPiece {
public:
    Bishop(string color) : ChessPiece(color, "BISHOP") {}

    bool isValidMove(int fromX, int fromY, int fromZ, int fromW, int toX, int toY, int toZ, int toW) const override {
        int dx = abs(toX - fromX);
        int dy = abs(toY - fromY);
        int dz = abs(toZ - fromZ);
        int dw = abs(toW - fromW);

        return (dx == dy && dy == dz && dz == dw);
    }
};

class Knight : public ChessPiece {
public:
    Knight(string color) : ChessPiece(color, "KNIGHT") {}

    bool isValidMove(int fromX, int fromY, int fromZ, int fromW, int toX, int toY, int toZ, int toW) const override {
        int dx = abs(toX - fromX);
        int dy = abs(toY - fromY);
        int dz = abs(toZ - fromZ);
        int dw = abs(toW - fromW);

        return (dx == 1 && dy == 2 && dz == 1 && dw == 2) ||
               (dx == 1 && dy == 2 && dz == 2 && dw == 1) ||
               (dx == 2 && dy == 1 && dz == 1 && dw == 2) ||
               (dx == 2 && dy == 1 && dz == 2 && dw == 1);
    }
};

class Pawn : public ChessPiece {
public:
    Pawn(string color) : ChessPiece(color, "PAWN") {}

    bool isValidMove(int fromX, int fromY, int fromZ, int fromW, int toX, int toY, int toZ, int toW) const override {
        int dx = toX - fromX;
        int dy = toY - fromY;
        int dz = toZ - fromZ;
        int dw = toW - fromW;

        // Movement rules for pawns
        if (getColor() == "WHITE") {
            return (dx == 1 && dy == 0 && dz == 0 && dw == 0) || // Move one step forward
                   (dx == 1 && dy == 0 && dz == 0 && dw == 1);    // Capture diagonally
        } else {
            return (dx == -1 && dy == 0 && dz == 0 && dw == 0) || // Move one step forward
                   (dx == -1 && dy == 0 && dz == 0 && dw == -1);   // Capture diagonally
        }
    }
};


// Class to represent the chess board
class ChessBoard {
public:
    ChessBoard() {
        // Initialize the 4D chess board with empty pieces
        board.resize(BOARD_SIZE, vector<vector<vector<ChessPiece*>>>(BOARD_SIZE,
            vector<vector<ChessPiece*>>(BOARD_SIZE, vector<ChessPiece*>(BOARD_SIZE, nullptr))));
    }

    // delete function
    ~ChessBoard() {
        // Iterate through the board to delete pieces
        for (int fromX = 0; fromX < BOARD_SIZE; fromX++) {
            for (int fromY = 0; fromY < BOARD_SIZE; fromY++) {
                for (int fromZ = 0; fromZ < BOARD_SIZE; fromZ++) {
                    for (int fromW = 0; fromW < BOARD_SIZE; fromW++) {
                        ChessPiece* piece = board[fromX][fromY][fromZ][fromW];
                        if (piece != nullptr) {
                            delete piece;
                            board[fromX][fromY][fromZ][fromW] = nullptr;
                        }
                    }
                }
            }
        }
    }

    // Function to place a piece on the board
    void placePiece(int x, int y, int z, int w, ChessPiece* piece) {
        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE ||
            z < 0 || z >= BOARD_SIZE || w < 0 || w >= BOARD_SIZE) {
            // Handle invalid coordinates
            cout << "Invalid coordinates for placing the piece." << endl;
            return;
        }

        if (board[x][y][z][w] != nullptr) {
            // Handle the case where there's already a piece at the destination
            cout << "There is already a piece at the destination." << endl;
            return;
        }

        board[x][y][z][w] = piece;
    }

    void initializeBoard() {
        // Set all pawn positions
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[x][1][0][0] = new Pawn("WHITE"); // Player 1's pawns
            board[x][6][0][0] = new Pawn("BLACK"); // Player 2's pawns
        }

        // Set all other White piece positions
        board[0][0][0][0] = new Rook("WHITE");
        board[7][0][0][0] = new Rook("WHITE");
        board[1][0][0][0] = new Knight("WHITE");
        board[6][0][0][0] = new Knight("WHITE");
        board[2][0][0][0] = new Bishop("WHITE");
        board[5][0][0][0] = new Bishop("WHITE");
        board[3][0][0][0] = new Queen("WHITE");
        board[4][0][0][0] = new King("WHITE");

        // Set all other Black piece positions
        board[0][7][0][0] = new Rook("BLACK");
        board[7][7][0][0] = new Rook("BLACK");
        board[1][7][0][0] = new Knight("BLACK");
        board[6][7][0][0] = new Knight("BLACK");
        board[2][7][0][0] = new Bishop("BLACK");
        board[5][7][0][0] = new Bishop("BLACK");
        board[3][7][0][0] = new Queen("BLACK");
        board[4][7][0][0] = new King("BLACK");


    }

    // Function to move a piece on the board
    bool movePiece(int fromX, int fromY, int fromZ, int fromW, int toX, int toY, int toZ, int toW) {
        ChessPiece* capturedPiece = board[toX][toY][toZ][toW];
        ChessPiece* piece = board[fromX][fromY][fromZ][fromW];
        // Assuming you have an external function isValidMove that checks the validity of the move
        if (piece->isValidMove(fromX, fromY, fromZ, fromW, toX, toY, toZ, toW)) {
            if (capturedPiece != nullptr) {
                if (capturedPiece->getColor() == piece->getColor()){
                    cout << "Cannot capture your own piece";
                    return false;
                }
                else{
                    delete capturedPiece; // Capturing a piece
                }
            }
            board[toX][toY][toZ][toW] = piece;
            board[fromX][fromY][fromZ][fromW] = nullptr;
            return true;
        }
        return false;
    }

    // Function to check if a square is attacked by the opponent
    bool isSquareDanger(int x, int y, int z, int w, string defenderColor) {
        string attackerColor;
        if (defenderColor == "WHITE"){attackerColor = "BLACK";}
            else {attackerColor = "WHITE";}
        
        // Iterate through the board to check if any opponent's piece can attack the square
        for (int fromX = 0; fromX < BOARD_SIZE; fromX++) {
            for (int fromY = 0; fromY < BOARD_SIZE; fromY++) {
                for (int fromZ = 0; fromZ < BOARD_SIZE; fromZ++) {
                    for (int fromW = 0; fromW < BOARD_SIZE; fromW++) {
                        ChessPiece* piece = board[fromX][fromY][fromZ][fromW];
                        if (piece != nullptr && piece->getColor() == attackerColor) {
                            // Check if the piece can attack the specified square
                            if (piece->isValidMove(fromX, fromY, fromZ, fromW, x, y, z, w)) {
                                return true; // The square is attacked by an opponent's piece
                            }
                        }
                    }
                }
            }
        }

        return false; // The square is not attacked by any opponent's piece
    }

    // Function to check if a player's king is in check
    bool isCheck(string color) {
        int kingX, kingY, kingZ, kingW;
        // Find the coordinates of the player's king
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                for (int z = 0; z < BOARD_SIZE; z++) {
                    for (int w = 0; w < BOARD_SIZE; w++) {
                        ChessPiece* piece = board[x][y][z][w];
                        if (piece != nullptr && piece->getColor() == color && piece->getType() == "KING") {
                            kingX = x;
                            kingY = y;
                            kingZ = z;
                            kingW = w;
                        }
                    }
                }
            }
        }

        // Check if any opponent's piece can attack the king
        return isSquareDanger(kingX, kingY, kingZ, kingW, color);
    }

    // Function to check if a player is in checkmate
    bool isCheckmate(string color) {
        int kingX, kingY, kingZ, kingW;

        // Find the coordinates of the player's king
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                for (int z = 0; z < BOARD_SIZE; z++) {
                    for (int w = 0; w < BOARD_SIZE; w++) {
                        ChessPiece* piece = board[x][y][z][w];
                        if (piece != nullptr && piece->getColor() == color && piece->getType() == "KING") {
                            kingX = x;
                            kingY = y;
                            kingZ = z;
                            kingW = w;
                        }
                    }
                }
            }
        }

        if (!isSquareDanger(kingX, kingY, kingZ, kingW, color)){return false;}

        // Check if the piece can move to a safe square
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    for (int dw = -1; dw <= 1; dw++) {
                        int toX = kingX + dx;
                        int toY = kingY + dy;
                        int toZ = kingZ + dz;
                        int toW = kingW + dw;

                        if (board[kingX][kingY][kingZ][kingW]->isValidMove(kingX, kingY, kingZ, kingW, toX, toY, toZ, toW)) {
                            // If the king can move to a safe square, it's not checkmate
                            if (!isSquareDanger(toX, toY, toZ, toW, color)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }

        // Check if any other piece can block the check or capture the attacking piece
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                for (int z = 0; z < BOARD_SIZE; z++) {
                    for (int w = 0; w < BOARD_SIZE; w++) {
                        ChessPiece* piece = board[x][y][z][w];
                        if (piece != nullptr && piece->getColor() == color) {
                            for (int toX = 0; toX < BOARD_SIZE; toX++) {
                                for (int toY = 0; toY < BOARD_SIZE; toY++) {
                                    for (int toZ = 0; toZ < BOARD_SIZE; toZ++) {
                                        for (int toW = 0; toW < BOARD_SIZE; toW++) {
                                            if (board[x][y][z][w]->isValidMove(x, y, z, w, toX, toY, toZ, toW) &&
                                                !isSquareDanger(toX, toY, toZ, toW, color)) {
                                                return false; // A piece can make a valid move to escape check
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return true; // No legal moves can get the king out of checkmate
    }

    private:
        vector<vector<vector<vector<ChessPiece*>>>> board;
};

int main() {
    // Create a 4D chess board
    ChessBoard chessBoard;

    //initialize pieces on board
    chessBoard.initializeBoard();

    // Move a piece (e.g., white king)
    chessBoard.movePiece(0, 0, 0, 0, 1, 0, 0, 0);

    //delete the board
    chessBoard.~ChessBoard();

    return 0;
}
