#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cctype>

const int BOARD_SIZE = 8;
const int count_of_moves = 3;

const char KING = 'K';
const char ROOK = 'R';
const char BISHOP = 'B';
const char KNIGHT = 'N';
const char PAWN = 'P';
const char EMPTY = '.';

class Piece {
public:
    char symbol;
    bool isWhite;

    // ����������� �� ���������
    Piece() : symbol(EMPTY), isWhite(true) {}

    // ����������� � �����������
    Piece(char sym, bool white) : symbol(sym), isWhite(white) {}

    // ����������� �����������
    Piece(const Piece& other) : symbol(other.symbol), isWhite(other.isWhite) {}

    // ���������� ��������� ������������
    Piece& operator=(const Piece& other) {
        if (this == &other) {
            return *this;
        }
        symbol = other.symbol;
        isWhite = other.isWhite;
        return *this;
    }
};

// ��������� ��������� ��������� ������� ��������� �����
void generateRandomPosition(std::vector<std::vector<Piece>>& board, int arr[]) {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            board[row][col] = Piece(EMPTY, true);
        }
    }

    int maxPieces[8];

    for (int p = 0; p < 8; ++p) {
        maxPieces[p] = arr[p];
    }

    // ���������� ������� �� �����
    int blackKingRow = std::rand() % 2;
    int whiteKingRow = BOARD_SIZE - (std::rand() % 2) - 1;
    int blackKingCol = std::rand() % BOARD_SIZE;
    int whiteKingCol = std::rand() % BOARD_SIZE;

    board[whiteKingRow][whiteKingCol] = Piece(KING, true);
    board[blackKingRow][blackKingCol] = Piece('k', false);

    while (maxPieces[0] != 0 || maxPieces[1] != 0 || maxPieces[2] != 0 || maxPieces[3] != 0 || maxPieces[4] != 0 || maxPieces[5] != 0 || maxPieces[6] != 0 || maxPieces[7] != 0) {

        // ���������� ����� ���������� ��������
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {

                if ((row == blackKingRow && col == blackKingCol) || (row == whiteKingRow && col == whiteKingCol)) {
                    continue;
                }

                if (board[row][col].symbol == EMPTY) {
                    int pieceChoice = std::rand() % 8;

                    if (maxPieces[pieceChoice] > 0) {
                        char pieceSymbol = '.';
                        bool isWhite = true;

                        if (pieceChoice < 4) {
                            pieceSymbol = pieceChoice == 0 ? 'R' : (pieceChoice == 1 ? 'B' : (pieceChoice == 2 ? 'N' : 'P'));
                        }
                        else {
                            pieceSymbol = pieceChoice == 4 ? 'r' : (pieceChoice == 5 ? 'b' : (pieceChoice == 6 ? 'n' : 'p'));
                            isWhite = false;
                        }

                        if (!(pieceSymbol == 'P' && row == 0) && !(pieceSymbol == 'p' && row == BOARD_SIZE - 1)) {
                            board[row][col] = Piece(pieceSymbol, isWhite);
                            maxPieces[pieceChoice]--;
                        }
                    }
                }
            }
        }
    }
}

// ����� ��������� �����
void printBoard(const std::vector<std::vector<Piece>>& board) {
    std::cout << "\\ " << "A " << "B " << "C " << "D " << "E " << "F " << "G " << "H " << std::endl;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        std::cout << row + 1 << ' ';
        for (int col = 0; col < BOARD_SIZE; ++col) {
            char symbol = board[row][col].symbol;
            if (board[row][col].isWhite) {
                std::cout << static_cast<char>(std::toupper(symbol)) << ' ';
            }
            else {
                std::cout << static_cast<char>(std::tolower(symbol)) << ' ';
            }
        }
        std::cout << std::endl;
    }
}

// ��������, �������� �� ������ ����������
bool isValidCell(int row, int col) {
    return row >= 0 && row < BOARD_SIZE&& col >= 0 && col < BOARD_SIZE;
}

// �������� ������������ ���� ��� �����
bool isValidPawnMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    if (isWhite && toRow == fromRow - 1 && toCol == fromCol && board[toRow][toCol].symbol == EMPTY) {
        return true;
    }
    else if (!isWhite && toRow == fromRow + 1 && toCol == fromCol && board[toRow][toCol].symbol == EMPTY) {
        return true;
    }

    if (isWhite && fromRow == 6 && toRow == 4 && toCol == fromCol && board[5][fromCol].symbol == EMPTY && board[4][fromCol].symbol == EMPTY) {
        return true;
    }
    else if (!isWhite && fromRow == 1 && toRow == 3 && toCol == fromCol && board[2][fromCol].symbol == EMPTY && board[3][fromCol].symbol == EMPTY) {
        return true;
    }

    if (isWhite && toRow == fromRow - 1 && (toCol == fromCol + 1 || toCol == fromCol - 1) && islower(board[toRow][toCol].symbol)) {
        return true;
    }
    else if (!isWhite && toRow == fromRow + 1 && (toCol == fromCol + 1 || toCol == fromCol - 1) && isupper(board[toRow][toCol].symbol)) {
        return true;
    }

    return false;
}

// �������� ������������ ���� ��� ������
bool isValidKingMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    int dr = std::abs(toRow - fromRow);
    int dc = std::abs(toCol - fromCol);
    return dr <= 1 && dc <= 1;
}

// �������� ������������ ���� ��� �����
bool isValidBishopMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    if (!isValidCell(fromRow, fromCol) || !isValidCell(toRow, toCol)) {
        return false;
    }

    char piece = std::toupper(board[fromRow][fromCol].symbol);

    int dr = toRow - fromRow;
    int dc = toCol - fromCol;

    if (std::abs(dr) != std::abs(dc)) {
        return false;
    }

    int rowStep = (dr > 0) ? 1 : -1;
    int colStep = (dc > 0) ? 1 : -1;

    int row = fromRow + rowStep;
    int col = fromCol + colStep;

    while (row != toRow || col != toCol) {
        if (row < 0 || row >= BOARD_SIZE) {
            return false;
        }

        if (col < 0 || col >= BOARD_SIZE) {
            return false;
        }

        if (board[row][col].symbol != EMPTY) {
            return false;
        }
        row += rowStep;
        col += colStep;
    }

    return true;
}


bool isValidRookMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    // ��������, ��� ��������� � �������� ������ ��������� � �������� �����
    if (!isValidCell(fromRow, fromCol) || !isValidCell(toRow, toCol)) {
        return false;
    }

    char piece = std::toupper(board[fromRow][fromCol].symbol);

    int dr = toRow - fromRow;
    int dc = toCol - fromCol;

    // ��������, ��� �������� �������������� �� ����������� ��� ���������
    if (dr != 0 && dc != 0) {
        return false;
    }

    int rowStep = (dr > 0) ? 1 : (dr < 0) ? -1 : 0;
    int colStep = (dc > 0) ? 1 : (dc < 0) ? -1 : 0;

    int row = fromRow + rowStep;
    int col = fromCol + colStep;

    // ��������, ��� ��� ������ ����� ��������� � �������� ������
    while (row != toRow || col != toCol) {
        if (board[row][col].symbol != EMPTY) {
            return false;  // ���� ������ �� ������
        }
        row += rowStep;
        col += colStep;
    }

    return true;
}

bool isValidKnightMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    int dr = std::abs(toRow - fromRow);
    int dc = std::abs(toCol - fromCol);
    return (dr == 1 && dc == 2) || (dr == 2 && dc == 1);
}

bool isValidMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    if (!isValidCell(fromRow, fromCol) || !isValidCell(toRow, toCol)) {
        return false;
    }

    if (board[toRow][toCol].isWhite == isWhite) {
        return false;  // ��������, ��� �������� ������ �� �������� ������ ���� �� �����
    }

    const Piece& movingPiece = board[fromRow][fromCol];
    const Piece& targetPiece = board[toRow][toCol];

    if (movingPiece.symbol == EMPTY || movingPiece.isWhite != true) {
        return false;
    }

    char piece = std::toupper(movingPiece.symbol);

    if (piece == PAWN) {
        return isValidPawnMove(board, fromRow, fromCol, toRow, toCol, true);
    }
    else if (piece == KING) {
        return isValidKingMove(board, fromRow, fromCol, toRow, toCol, true);
    }
    else if (piece == BISHOP) {
        return isValidBishopMove(board, fromRow, fromCol, toRow, toCol, true);
    }
    else if (piece == ROOK) {
        return isValidRookMove(board, fromRow, fromCol, toRow, toCol, true);
    }
    else if (piece == KNIGHT) {
        return isValidKnightMove(board, fromRow, fromCol, toRow, toCol, true);
    }

    return false;
}


bool isCheck(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, bool isWhite) {
    // �������� ������� ���� ��� ����� isWhite
    char kingSymbol = isWhite ? 'K' : 'k';
    int kingRow = -1;
    int kingCol = -1;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (board[row][col].symbol == kingSymbol) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
        if (kingRow != -1) {
            break;
        }
    }

    // ��������, ��� ������ �� ��������� ������ �� ���� �� �����, � ������ �� ������
    if (board[fromRow][fromCol].isWhite != isWhite && board[fromRow][fromCol].symbol != EMPTY) {
        if (isValidMove(board, fromRow, fromCol, kingRow, kingCol, !isWhite)) {
            return true;  // ��� ���������
        }
    }

    return false;  // ��� ���� ��� ������� �����
}

bool isCheckMate(std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, bool isWhite) {
    // �������� ������� ���� ��� ����� isWhite
    if (!isCheck(board, fromRow, fromCol, isWhite)) {
        return false;  // ��� ����, ������ ��� ����
    }

    // �������� ��������� ����� ��� ��������� ����
    if (board[fromRow][fromCol].isWhite == isWhite) {
        for (int toRow = 0; toRow < BOARD_SIZE; ++toRow) {
            for (int toCol = 0; toCol < BOARD_SIZE; ++toCol) {
                if (isValidMove(board, fromRow, fromCol, toRow, toCol, !isWhite)) {
                    // �������������� ��� ��� ��������� ����
                    Piece originalPiece = board[toRow][toCol];
                    board[toRow][toCol] = board[fromRow][fromCol];
                    board[fromRow][fromCol] = Piece(EMPTY, true);

                    // ��������, ��� ����� ���� ��� ����
                    if (!isCheck(board, fromRow, fromCol, isWhite)) {
                        // ���� ��� ����, ���������� false
                        board[fromRow][fromCol] = board[toRow][toCol];
                        board[toRow][toCol] = originalPiece;
                        return false;  // �������������� ��� �������� ����
                    }

                    board[fromRow][fromCol] = board[toRow][toCol];
                    board[toRow][toCol] = originalPiece;
                }
            }
        }
    }

    return true;  // ��� ��������� ����� ��� ��������� ����
}



bool canCapturePiece(const std::vector<std::vector<Piece>>& board, int row, int col, bool isWhite) {
    // �������� ����������� ������� ������ �� ��������� �������

    if (!isValidCell(row, col) || board[row][col].symbol == EMPTY) {
        return false;  // ������ ����� ��� �����������
    }

    if (board[row][col].isWhite == isWhite) {
        return false;  // ������ ���� �� �����
    }

    // ��������� ������� ��������� ������
    char attackingPiece = std::toupper(board[row][col].symbol);

    if (attackingPiece == KING) {
        return false;  // ������ �� ����� ���� ��������
    }

    return true;  // ����� ��������� ������ �� ��������� �������
}

bool canSavePiece(const std::vector<std::vector<Piece>>& board, int row, int col, bool isWhite) {
    // �������� ����������� ������ ����� ������ �� ��������� �������

    if (!isValidCell(row, col) || board[row][col].symbol == EMPTY || board[row][col].isWhite != isWhite) {
        return false;  // ������ �����, ������ �� ������ ����� ��� ������������
    }

    if (board[row][col].isWhite == isWhite) {
        return false;  // ������ ���� �� �����
    }

    // ��������� ������� ������
    char piece = std::toupper(board[row][col].symbol);
    return piece != KING;  // ������ �� ����� ���� �������
}

int getPieceValue(char pieceSymbol) {
    // ��������� �������� ������ ��� ������ ������� � ����

    switch (pieceSymbol) {
    case PAWN:
        return 1;
    case KNIGHT:
        return 3;
    case BISHOP:
        return 3;
    case ROOK:
        return 5;
    case KING:
        return 1000;
    default:
        return 0;  // ����������� ������
    }
}

bool isValidCoordinate(int row, int col) {
    // �������� ������������ ���������� �� �����

    return row >= 0 && row < BOARD_SIZE&& col >= 0 && col < BOARD_SIZE;
}

int evaluateMove(std::vector<std::vector<Piece>> board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    // ������ ���� � ���� ��� ����������� ��� ��������

    if (!isValidCoordinate(fromRow, fromCol) || !isValidCoordinate(toRow, toCol)) {
        // ������������ ����������
        return -1000; // ���������� �������� ��� ������������� ����
    }

    // ���������� ������, ������� �� ������ ����������, � ����������� ������
    Piece originalPiece = board[toRow][toCol];
    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = Piece(EMPTY, true);

    // �������� ������� ������� �������� ����� ����
    if (isCheckMate(board, fromRow, fromCol, isWhite)) {
        if (board[toRow][toCol].isWhite != isWhite) {
            // ������� �������� �������, ���� ��� �� �������� ����
            board[fromRow][fromCol] = board[toRow][toCol];
            board[toRow][toCol] = originalPiece;
            return 5000; // �������, ��� ��� �������� ���
        }
    }

    // �������� ������� ���� ����� ����
    if (isCheck(board, fromRow, fromCol, isWhite)) {
        if (board[toRow][toCol].isWhite != isWhite) {
            // ������� �������� �������, ���� ��� �� �������� ����
            board[fromRow][fromCol] = board[toRow][toCol];
            board[toRow][toCol] = originalPiece;
            return 2000; // �������, ��� ��� �������� ���
        }
    }

    // �������� ����������� ������� ������ ��� ���� ����� ����
    if (canCapturePiece(board, toRow, toCol, isWhite) && !isCheck(board, fromRow, fromCol, isWhite)) {
        if (board[toRow][toCol].isWhite != isWhite) {
            // ������� �������� �������, ���� ��� �� �������� ������� ������ ��� ����
            board[fromRow][fromCol] = board[toRow][toCol];
            board[toRow][toCol] = originalPiece;
            return 1000; // �������, ��� ��� ��������� ��������� ������ ��� ����
        }
    }

    // �������� ����������� ������� ������ � ����� ����� ����
    if (canCapturePiece(board, toRow, toCol, isWhite) && isCheck(board, fromRow, fromCol, !isWhite)) {
        if (board[toRow][toCol].isWhite != isWhite) {
            if (board[fromRow][fromCol].symbol == KING) return 0;
            // ������� �������� �������, ���� ��� �� �������� ������� ������ � �����
            board[fromRow][fromCol] = board[toRow][toCol];
            board[toRow][toCol] = originalPiece;
            return 100; // �������, ��� ��� ��������� ��������� ������ � �����
        }
    }

    // �������� ����������� ������ ����� ������ ����� ����
    if (canSavePiece(board, toRow, toCol, isWhite)) {
        if (board[toRow][toCol].isWhite != isWhite) {
            // ������� �������� �������, ���� ��� �� �������� ������ ����� ������
            board[fromRow][fromCol] = board[toRow][toCol];
            board[toRow][toCol] = originalPiece;
            return 500; // �������, ��� ��� ��������� �������� ���� ������
        }
    }

    if (board[toRow][toCol].isWhite != isWhite) {
        // ��� �� �������� � ������� ������ ��� ������ ����� ������
        board[fromRow][fromCol] = board[toRow][toCol];
        board[toRow][toCol] = originalPiece;
        return 0; // ����������� ��� ��� ��������� � �������
    }

    // ����������� ��� ��� ��������� � �������
    return -100;
}


std::vector<std::pair<int, int>> findBestMoves(const std::vector<std::vector<Piece>>& board, bool isWhite, int num) {
    // ����� ������ ���� ��� �������� ������
    std::vector<std::pair<int, int>> allMoves;

    // ������� ���� ������ �� �����
    for (int fromRow = 0; fromRow < BOARD_SIZE; ++fromRow) {
        for (int fromCol = 0; fromCol < BOARD_SIZE; ++fromCol) {
            if (board[fromRow][fromCol].isWhite == isWhite) {
                for (int toRow = 0; toRow < BOARD_SIZE; ++toRow) {
                    for (int toCol = 0; toCol < BOARD_SIZE; ++toCol) {
                        // �������� ���������� ���� � ���� �����
                        if (isValidMove(board, fromRow, fromCol, toRow, toCol, isWhite) && board[fromRow][fromCol].symbol != '.' && board[toRow][toCol].symbol != std::toupper(KING) && board[toRow][toCol].symbol != std::tolower(KING)) {
                            allMoves.push_back(std::make_pair(fromRow * 10 + fromCol, toRow * 10 + toCol));
                        }
                    }
                }
            }
        }
    }

    std::vector<std::pair<int, int>> bestMoves;

    // ������ ����� � ������� evaluateMove
    std::sort(allMoves.begin(), allMoves.end(), [&](const std::pair<int, int>& move1, const std::pair<int, int>& move2) {
        int eval1 = evaluateMove(board, move1.first / 10, move1.first % 10, move1.second / 10, move1.second % 10, isWhite);
    int eval2 = evaluateMove(board, move2.first / 10, move2.first % 10, move2.second / 10, move2.second % 10, isWhite);
    return eval1 > eval2;
        });

    // ����� ������ �����
    for (size_t i = 0; i < num; ++i) {
        if (i < allMoves.size()) {
            bestMoves.push_back(allMoves[i]);
        }
        else {
            break;
        }
    }

    return bestMoves;
}

void printMoves(const std::vector<std::vector<Piece>>& board, const std::vector<std::pair<int, int>>& moves) {
    int move_index = 1;
    for (const auto& move : moves) {
        int fromRow = move.first / 10;
        int fromCol = move.first % 10;
        int toRow = move.second / 10;
        int toCol = move.second % 10;

        const char colLetters[8]{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

        if (isValidCell(fromRow, fromCol) && isValidCell(toRow, toCol)) {
            std::cout << move_index << ") " << board[fromRow][fromCol].symbol << ": " << colLetters[fromCol] << fromRow + 1 << " - " << colLetters[toCol] << toRow + 1 << std::endl;
            move_index++;
        }
        else {
            // ����� ��������� �� ������, ���� ���������� ���������������
            std::cout << "������: ������������ ���������� ����" << std::endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<std::pair<int, int>> bestMoves;
    bool isWhiteToMove, success, flag, isCheckPresent;
    int fromRow, fromCol, fr, fc, tr, tc, move_i;
    const char colLetters[8]{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

    std::vector<std::vector<Piece>> board(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE));

    // ��������� ��������� ������� �����
    isWhiteToMove = true; // ��� �����

    fromRow = 0;
    fromCol = 0;

    int maxPieces[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    std::cout << "=== ��������� ��������� ������� ����� ===" << std::endl;
    std::cout << "������� ���������� ����� ������: ";
    std::cin >> maxPieces[0];
    std::cout << std::endl;
    std::cout << "������� ���������� �����  �����: ";
    std::cin >> maxPieces[1];
    std::cout << std::endl;
    std::cout << "������� ���������� �����  ������: ";
    std::cin >> maxPieces[2];
    std::cout << std::endl;
    std::cout << "������� ���������� �����  �����: ";
    std::cin >> maxPieces[3];
    std::cout << std::endl;
    std::cout << "������� ���������� ������  ������: ";
    std::cin >> maxPieces[4];
    std::cout << std::endl;
    std::cout << "������� ���������� ������ �����: ";
    std::cin >> maxPieces[5];
    std::cout << std::endl;
    std::cout << "������� ���������� ������ ������: ";
    std::cin >> maxPieces[6];
    std::cout << std::endl;
    std::cout << "������� ���������� ������ �����: ";
    std::cin >> maxPieces[7];
    std::cout << std::endl;

    generateRandomPosition(board, maxPieces);

    // ����� � ���������� �����

    bestMoves = findBestMoves(board, isWhiteToMove, count_of_moves);

    // ���� ���������� ����� �� ���������� ����
    while (fromRow < BOARD_SIZE) {
        success = true;
        while (fromCol < BOARD_SIZE) {
            if (isCheck(board, fromRow, fromCol, !isWhiteToMove) || isCheck(board, fromRow, fromCol, isWhiteToMove) || bestMoves.size() < count_of_moves) {
                generateRandomPosition(board, maxPieces);
                bestMoves = findBestMoves(board, isWhiteToMove, count_of_moves);
                fromRow = 0;
                fromCol = 0;
                success = false;
                break;
            }
            else {
                fromCol++;
            }
        }
        if (success) {
            fromCol = 0;
            fromRow++;
        }
    }

    // ����������� ������ �����
    std::vector<std::pair<int, int>> origBM(bestMoves);
    std::vector<std::pair<int, int>> madeMoves;

    madeMoves.push_back(origBM[0]);
    madeMoves.push_back(origBM[1]);
    madeMoves.push_back(origBM[2]);

    std::cout << "������� ��������� �����: " << std::endl;
    printBoard(board);

    std::cout << std::endl;
    std::cout << "������ ����:" << std::endl;
    printMoves(board, bestMoves);
    std::cout << std::endl << std::endl;

    for (int i = 0; i < 3; ++i) {

        std::vector<std::vector<Piece>> temp_board(board);

        std::cout << "��� ����� " << i + 1 << ": " << std::endl << std::endl;

        fr = origBM[i].first / 10;
        fc = origBM[i].first % 10;
        tr = origBM[i].second / 10;
        tc = origBM[i].second % 10;

        std::cout << temp_board[fr][fc].symbol << ": " << colLetters[fc] << fr + 1 << " - " << colLetters[tc] << tr + 1 << std::endl;

        temp_board[tr][tc].symbol = temp_board[fr][fc].symbol;
        temp_board[tr][tc].isWhite = true;
        temp_board[fr][fc].symbol = '.';

        printBoard(temp_board);
        std::cout << std::endl << std::endl;

        flag = true;

        for (int j = 0; j < 2; ++j) {

            bestMoves = findBestMoves(temp_board, isWhiteToMove, count_of_moves * 3);
            move_i = 0;

            isCheckPresent = false;

            for (int row = 0; row < BOARD_SIZE; ++row) {
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    if (isCheck(temp_board, row, col, !isWhiteToMove)) {
                        isCheckPresent = true;
                    }
                }
            }

            if (isCheckPresent) {
                std::cout << "���!" << std::endl << std::endl;
                break;
            }
            else {
                while (flag && count(madeMoves.begin(), madeMoves.end(), bestMoves[move_i]) > 0) {
                    move_i++;
                    if (move_i == bestMoves.size()) {
                        std::cout << "���� ��� ������ ������� �����������!" << std::endl << std::endl;
                        flag = false;
                    }
                }
            }

            madeMoves.push_back(bestMoves[move_i]);

            fr = bestMoves[move_i].first / 10;
            fc = bestMoves[move_i].first % 10;
            tr = bestMoves[move_i].second / 10;
            tc = bestMoves[move_i].second % 10;

            std::cout << temp_board[fr][fc].symbol << ": " << colLetters[fc] << fr + 1 << " - " << colLetters[tc] << tr + 1 << std::endl;

            temp_board[tr][tc].symbol = temp_board[fr][fc].symbol;
            temp_board[tr][tc].isWhite = true;
            temp_board[fr][fc].symbol = '.';

            printBoard(temp_board);
            std::cout << std::endl << std::endl;

        }

    }

    return 0;
}
