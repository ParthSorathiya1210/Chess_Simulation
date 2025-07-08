#include <bits/stdc++.h>
using namespace std;

#define left 0
#define right 7
#define top 7
#define bottom 0
#define Size 8

// 0 - 7 white Pawn
// 8 - 9 white Rook
// 10 - 11 white Knight
// 12 - 13 white Bishop
// 14 - white Queen
// 15 - white King

// 16 - 23 black Pawn
// 24 - 25 black Rook
// 26 - 27 black Knight
// 28 - 29 black Bishop
// 30 - black King
// 31 - black Queen

class Pieces
{
protected:
    int name;    // -1 represents the blank place.
    bool colour; // white -  0 // black - 1
    bool alive;
    bool is_first_move;
    bool is_last_square_move = false;
    pair<int, int> position;

public:
    Pieces(int name, bool colour, pair<int, int> position)
    {
        alive = 1;
        this->name = name;
        this->colour = colour;
        this->position = position;
        is_first_move = true;
    }

    int getName()
    {
        return name;
    }

    int getColour()
    {
        return colour;
    }

    pair<int, int> getPosition()
    {
        return position;
    }

    bool isAlive()
    {
        return alive;
    }

    void Kill()
    {
        alive = false;
    }

    bool isFirstMove()
    {
        return is_first_move;
    }

    bool isLastSquareMove()
    {
        return is_last_square_move;
    }

    virtual bool is_valid_move(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) = 0;
    virtual bool check_obstacle(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) = 0;
    virtual void move_position(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) = 0;
    // const functions can not change the variables of the object except the varible itself is mutable like mutable int a;
    virtual Pieces *clone() const = 0;

    static bool check_boundry(pair<int, int> new_position)
    {
        int X = new_position.first;
        int Y = new_position.second;

        if (X >= left && X <= right && Y >= bottom && Y <= top)
            return false;
        else
            return true;
    }

    static pair<string, bool> which_piece(int num)
    {
        if (num >= 0 && num <= 7)
            return {"Pawn", 0};
        else if (num >= 8 && num <= 9)
            return {"Rook", 0};
        else if (num >= 10 && num <= 11)
            return {"Knight", 0};
        else if (num >= 12 && num <= 13)
            return {"Bishop", 0};
        else if (num >= 14 && num <= 14)
            return {"Queen", 0};
        else if (num >= 15 && num <= 15)
            return {"King", 0};
        else if (num >= 16 && num <= 23)
            return {"Pawn", 1};
        else if (num >= 24 && num <= 25)
            return {"Rook", 1};
        else if (num >= 26 && num <= 27)
            return {"Knight", 1};
        else if (num >= 28 && num <= 29)
            return {"Bishop", 1};
        else if (num >= 30 && num <= 30)
            return {"King", 1};
        else if (num >= 31 && num <= 31)
            return {"Queen", 1};
        else
            return {"blank", 0};
    }

    static vector<vector<Pieces *>> deepCopyBoard(vector<vector<Pieces *>> &originalBoard)
    {
        int rows = originalBoard.size();
        int cols = originalBoard[0].size();

        vector<vector<Pieces *>> newBoard(rows, vector<Pieces *>(cols));

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                newBoard[i][j] = originalBoard[i][j]->clone();
            }
        }

        return newBoard;
    }

    static void deleteBoard(vector<vector<Pieces *>> &board)
    {
        for (auto &row : board)
        {
            for (Pieces *piece : row)
            {
                delete piece;
            }
        }
    }

protected:
    static void handleEnPesantFalse(vector<vector<Pieces *>> &chessBoard)
    {
        for (int i = 0; i < Size; ++i)
        {
            for (int j = 0; j < Size; ++j)
            {
                chessBoard[i][j]->is_last_square_move = false;
            }
        }
    }
};

class Blank : public Pieces
{
public:
    Blank(int name, bool colour, pair<int, int> position) : Pieces(name, colour, position) {}

    virtual Pieces *clone() const override
    {
        Blank *newCopy = new Blank(name, colour, position);
        newCopy->alive = alive;
        newCopy->is_first_move = is_first_move;
        newCopy->is_last_square_move = is_last_square_move;
        return newCopy;
    }

    bool is_valid_move(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        return false;
    }

private:
    bool check_obstacle(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        return true;
    }

    void move_position(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        return;
    }
};

class King : public Pieces
{
public:
    King(int name, bool colour, pair<int, int> position) : Pieces(name, colour, position) {}

    King *clone() const override
    {
        King *newCopy = new King(name, colour, position);
        newCopy->alive = alive;
        newCopy->is_first_move = is_first_move;
        newCopy->is_last_square_move = is_last_square_move;
        return newCopy;
    }

    bool is_check(vector<vector<Pieces *>> &chessBoard)
    {
        return is_check(chessBoard, position);
    }

    bool is_check(vector<vector<Pieces *>> &chessBoard, pair<int, int> position)
    {
        int curr_X = position.first;
        int curr_Y = position.second;

        // Bishop - Queen
        bool top_right = false;
        bool top_left = false;
        bool bottom_left = false;
        bool bottom_right = false;

        // Rook - Queen
        bool Top = false;
        bool Bottom = false;
        bool Left = false;
        bool Right = false;

        for (int i = 1; i <= Size; i++)
        {
            int j = i;
            if (!top_right && !check_boundry({curr_X + i, curr_Y + j}))
            {
                if (chessBoard[curr_X + i][curr_Y + j]->getName() != -1)
                {
                    if (colour != chessBoard[curr_X + i][curr_Y + j]->getColour())
                    {
                        int new_name = chessBoard[curr_X + i][curr_Y + j]->getName();
                        if (new_name == 12 || new_name == 13 || new_name == 28 || new_name == 29 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            top_right = true;
                    }
                    else
                        top_right = true;
                }
            }

            if (!top_left && !check_boundry({curr_X - i, curr_Y + j}))
            {
                if (chessBoard[curr_X - i][curr_Y + j]->getName() != -1)
                {
                    if (colour != chessBoard[curr_X - i][curr_Y + j]->getColour())
                    {
                        int new_name = chessBoard[curr_X - i][curr_Y + j]->getName();
                        if (new_name == 12 || new_name == 13 || new_name == 28 || new_name == 29 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            top_left = true;
                    }
                    else
                        top_left = true;
                }
            }

            if (!bottom_right && !check_boundry({curr_X + i, curr_Y - j}))
            {
                if (chessBoard[curr_X + i][curr_Y - j]->getName() != -1)
                {
                    if (colour != chessBoard[curr_X + i][curr_Y - j]->getColour())
                    {
                        int new_name = chessBoard[curr_X + i][curr_Y - j]->getName();
                        if (new_name == 12 || new_name == 13 || new_name == 28 || new_name == 29 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            bottom_right = true;
                    }
                    else
                        bottom_right = true;
                }
            }

            if (!bottom_left && !check_boundry({curr_X - i, curr_Y - j}))
            {
                if (chessBoard[curr_X - i][curr_Y - j]->getName() != -1)
                {
                    if (colour != chessBoard[curr_X - i][curr_Y - j]->getColour())
                    {
                        int new_name = chessBoard[curr_X - i][curr_Y - j]->getName();
                        if (new_name == 12 || new_name == 13 || new_name == 28 || new_name == 29 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            bottom_left = true;
                    }
                    else
                        bottom_left = true;
                }
            }

            if (!Right && !check_boundry({curr_X + i, curr_Y}))
            {
                if (chessBoard[curr_X + i][curr_Y]->getName() != -1)
                {
                    if (colour != chessBoard[curr_X + i][curr_Y]->getColour())
                    {
                        int new_name = chessBoard[curr_X + i][curr_Y]->getName();
                        if (new_name == 8 || new_name == 9 || new_name == 24 || new_name == 25 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            Right = true;
                    }
                    else
                        Right = true;
                }
            }

            if (!Left && !check_boundry({curr_X - i, curr_Y}))
            {
                if (chessBoard[curr_X - i][curr_Y]->getName() != -1)
                {
                    if (colour != chessBoard[curr_X - i][curr_Y]->getColour())
                    {
                        int new_name = chessBoard[curr_X - i][curr_Y]->getName();
                        if (new_name == 8 || new_name == 9 || new_name == 24 || new_name == 25 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            Left = true;
                    }
                    else
                        Left = true;
                }
            }

            if (!Top && !check_boundry({curr_X, curr_Y + i}))
            {
                if (chessBoard[curr_X][curr_Y + i]->getName() != -1)
                {
                    if (colour != chessBoard[curr_X][curr_Y + i]->getColour())
                    {
                        int new_name = chessBoard[curr_X][curr_Y + i]->getName();
                        if (new_name == 8 || new_name == 9 || new_name == 24 || new_name == 25 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            Top = true;
                    }
                    else
                        Top = true;
                }
            }

            if (!Bottom && !check_boundry({curr_X, curr_Y - i}))
            {
                if (chessBoard[curr_X][curr_Y - i]->getName() != -1)
                {
                    if (colour != chessBoard[curr_X][curr_Y - i]->getColour())
                    {
                        int new_name = chessBoard[curr_X][curr_Y - i]->getName();
                        if (new_name == 8 || new_name == 9 || new_name == 24 || new_name == 25 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            Bottom = true;
                    }
                    else
                        Bottom = true;
                }
            }
        }

        // pawn
        bool pawn_threat = false;
        if (colour == 0)
        {
            if ((!check_boundry({curr_X + 1, curr_Y + 1}) && (chessBoard[curr_X + 1][curr_Y + 1]->getName() >= 16 && chessBoard[curr_X + 1][curr_Y + 1]->getName() <= 23)) || (!check_boundry({curr_X + 1, curr_Y - 1}) && (chessBoard[curr_X + 1][curr_Y - 1]->getName() >= 16 && chessBoard[curr_X + 1][curr_Y - 1]->getName() <= 23)))
                pawn_threat = true;
        }
        else
        {
            if ((!check_boundry({curr_X - 1, curr_Y - 1}) && (chessBoard[curr_X - 1][curr_Y - 1]->getName() >= 0 && chessBoard[curr_X - 1][curr_Y - 1]->getName() <= 7)) || (!check_boundry({curr_X - 1, curr_Y + 1}) && (chessBoard[curr_X - 1][curr_Y + 1]->getName() >= 0 && chessBoard[curr_X - 1][curr_Y + 1]->getName() <= 7)))
                pawn_threat = true;
        }
        if (pawn_threat)
            return true;

        // Knight

        bool knight_threat = false;

        int row[8] = {2, 2, -2, -2, 1, 1, -1, -1};
        int col[8] = {1, -1, -1, 1, 2, -2, 2, -2};

        for (int i = 0; i < 8; i++)
        {
            if (!check_boundry({curr_X + row[i], curr_Y + col[i]}))
            {
                int new_name = chessBoard[curr_X + row[i]][curr_Y + col[i]]->getName();
                if (!knight_threat && new_name != -1 && colour != chessBoard[curr_X + row[i]][curr_Y + col[i]]->getColour() && (new_name == 10 || new_name == 11 || new_name == 26 || new_name == 27))
                    knight_threat = true;
            }
        }
        if (knight_threat)
            return true;

        // if none of the above check condition returns true then there is no check so we can return false;
        return false;
    }

    bool is_checkmate(vector<vector<Pieces *>> &chessBoard)
    {
        for (int i = 0; i < Size; i++)
        {
            for (int j = 0; j < Size; j++)
            {
                if (chessBoard[i][j]->getColour() == colour)
                {
                    for (int k = 0; k < Size; k++)
                    {
                        for (int l = 0; l < Size; l++)
                        {
                            if (i == k && l == j)
                                continue;
                            vector<vector<Pieces *>> copyBoard = deepCopyBoard(chessBoard);
                            if (copyBoard[i][j]->is_valid_move({k, l}, copyBoard))
                            {
                                pair<int, int> newPos = position;
                                if (which_piece(copyBoard[k][l]->getName()).first == "King")
                                {
                                    newPos.first = k;
                                    newPos.second = l;
                                }
                                if (!is_check(copyBoard, newPos))
                                {
                                    deleteBoard(copyBoard);
                                    return false;
                                }
                            }
                            deleteBoard(copyBoard);
                        }
                    }
                }
            }
        }

        // if function not returened yet false then is means that is checkmate hence return true
        return true;
    }

    bool is_stalemate(vector<vector<Pieces *>> &chessBoard, bool colour)
    {
        if (is_check(chessBoard, position))
        {
            return false;
        }

        for (int i = 0; i < Size; i++)
        {
            for (int j = 0; j < Size; j++)
            {
                if (chessBoard[i][j]->getColour() == colour)
                {
                    for (int k = 0; k < 8; k++)
                    {
                        for (int l = 0; l < 8; l++)
                        {
                            if (i == k && l == j)
                                continue;
                            vector<vector<Pieces *>> copyBoard = deepCopyBoard(chessBoard);
                            if (copyBoard[i][j]->is_valid_move({k, l}, copyBoard))
                            {
                                pair<int, int> newPos = position;
                                if (which_piece(copyBoard[k][l]->getName()).first == "King")
                                {
                                    newPos.first = k;
                                    newPos.second = l;
                                }
                                if (is_check(copyBoard, newPos))
                                {
                                    deleteBoard(copyBoard);
                                    continue;
                                }

                                deleteBoard(copyBoard);
                                return false;
                            }
                            deleteBoard(copyBoard);
                        }
                    }
                }
            }
        }

        // if function not returened yet false then is means that is stalemate hence return true
        return true;
    }

    bool check_obstacle(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        int new_X = new_position.first;
        int new_Y = new_position.second;

        bool curr_colour = this->colour;
        if (chessBoard[new_X][new_Y]->getName() == -1)
            return false;
        else if (chessBoard[new_X][new_Y]->getName() != -1 && chessBoard[new_X][new_Y]->getColour() != colour)
            return false;
        else
            return true;
    }

    bool is_valid_move(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (alive == false || check_boundry(new_position))
            return false;

        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        if (abs(new_X - curr_X) <= 1 && abs(new_Y - curr_Y) <= 1 && abs(new_X - curr_X) + abs(new_Y - curr_Y) != 0)
        {
            if (check_obstacle(new_position, chessBoard))
            {
                return false;
            }
            else
            {
                move_position(new_position, chessBoard);
                return true;
            }
        }
        else if (castling_condition(new_position, chessBoard)) // castling condition
        {
            move_position(new_position, chessBoard);
            return true;
        }
        else
            return false;
    }

private:
    bool castling_condition(pair<int, int> &new_position, vector<vector<Pieces *>> &chessBoard)
    {
        // if it's not kings first move
        if (is_first_move == false)
            return false;

        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        // castling condition  // keep white rook named 9 right side // keep white rook named 8 at left side
        if (colour == 0 && curr_X == 0 && curr_Y == 4 && abs(new_Y - curr_Y) == 2 && abs(new_X - curr_X) == 0)
        {
            if (new_Y > curr_Y)
            {
                for (int i = 1; i <= 2; i++)
                {

                    // if the path has obstacles or in the path there is check then return false
                    if (chessBoard[curr_X][curr_Y + i]->getName() != -1 || is_check(chessBoard, position))
                        return false;
                }
                if (chessBoard[curr_X][curr_Y + 3]->getName() != 9 || chessBoard[curr_X][curr_Y + 3]->isFirstMove() == false) // if its not rook or not its first move
                    return false;
            }
            else if (new_Y < curr_Y)
            {
                for (int i = 1; i <= 3; i++)
                {
                    if (chessBoard[curr_X][curr_Y - i]->getName() != -1 || is_check(chessBoard, position))
                        return false;
                }
                if (chessBoard[curr_X][curr_Y - 4]->getName() != 8 || chessBoard[curr_X][curr_Y - 4]->isFirstMove() == false)
                    return false;
            }
            return true;
        }
        else if (colour == 1 && curr_Y == 4 && curr_X == 7 && abs(new_Y - curr_Y) == 2 && abs(new_X - curr_X) == 0)
        {
            if (new_Y > curr_Y)
            {
                for (int i = 1; i <= 2; i++)
                {
                    if (chessBoard[curr_X][curr_Y + i]->getName() != -1 || is_check(chessBoard, position))
                        return false;
                }
                if (chessBoard[curr_X][curr_Y + 3]->getName() != 25 || chessBoard[curr_X][curr_Y + 3]->isFirstMove() == false)
                    return false;
            }
            else if (new_Y < curr_Y)
            {
                for (int i = 1; i <= 3; i++)
                {
                    if (chessBoard[curr_X][curr_Y - i]->getName() != -1 || is_check(chessBoard, position))
                        return false;
                }
                if (chessBoard[curr_X][curr_Y - 4]->getName() != 24 || chessBoard[curr_X][curr_Y - 4]->isFirstMove() == false)
                    return false;
            }
            return true;
        }
        else
            return false;
    }

    void move_position(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        handleEnPesantFalse(chessBoard);
        if (is_first_move == true)
            is_first_move = false;

        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        if (abs(new_Y - curr_Y) == 2) //  must be castling
        {
            Pieces *newPiece = chessBoard[new_X][new_Y];
            if (newPiece->getName() != -1)
                newPiece->Kill();

            chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
            position = new_position;
            chessBoard[curr_X][curr_Y] = new Blank(-1, 0, {-1, -1});

            Pieces *rook;
            Pieces *newplace;
            if (curr_X == 0 && new_Y == 6)
            {
                rook = chessBoard[0][7];
                newplace = chessBoard[0][5];
                chessBoard[0][5] = rook;
                rook->getPosition() = {0, 5};
                chessBoard[0][7] = newplace;
            }
            else if (curr_X == 0 && new_Y == 2)
            {
                rook = chessBoard[0][0];
                newplace = chessBoard[0][3];
                chessBoard[0][3] = rook;
                rook->getPosition() = {0, 3};
                chessBoard[0][0] = newplace;
            }
            else if (curr_X == 7 && new_Y == 6)
            {
                rook = chessBoard[7][7];
                newplace = chessBoard[7][5];
                chessBoard[7][5] = rook;
                rook->getPosition() = {7, 5};
                chessBoard[7][7] = newplace;
            }
            else if (curr_X == 7 && new_Y == 2)
            {
                rook = chessBoard[7][0];
                newplace = chessBoard[7][3];
                chessBoard[7][3] = rook;
                rook->getPosition() = {7, 3};
                chessBoard[7][0] = newplace;
            }
        }
        else
        {
            Pieces *newPiece = chessBoard[new_X][new_Y];
            if (newPiece->getName() != -1)
                newPiece->Kill();

            chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
            position = new_position;
            chessBoard[curr_X][curr_Y] = new Blank(-1, 0, {-1, -1});
        }
    }
};

class Rook : public Pieces
{
public:
    Rook(int name, bool colour, pair<int, int> position) : Pieces(name, colour, position) {}

    Rook *clone() const override
    {
        Rook *newCopy = new Rook(name, colour, position);
        newCopy->alive = alive;
        newCopy->is_first_move = is_first_move;
        newCopy->is_last_square_move = is_last_square_move;

        return newCopy;
    }

    bool is_valid_move(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (this->alive == false || check_boundry(new_position))
            return false;

        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        if (new_X != curr_X && new_Y != curr_Y)
            return false;
        else if (new_X == curr_X && new_Y == curr_Y)
            return false;
        else
        {
            if (check_obstacle(new_position, chessBoard))
                return false;
            else
            {
                move_position(new_position, chessBoard);
                return true;
            }
        }
    }

private:
    bool check_obstacle(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        if (new_X == curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (int i = curr_Y + 1; i < new_Y; i++)
                {
                    if (chessBoard[curr_X][i]->getName() != -1)
                        return true;
                }
            }
            else
            {
                for (int i = curr_Y - 1; i > new_Y; i--)
                {
                    if (chessBoard[curr_X][i]->getName() != -1)
                        return true;
                }
            }
        }
        else if (new_Y == curr_Y)
        {
            if (new_X > curr_X)
            {
                for (int i = curr_X + 1; i < new_X; i++)
                {
                    if (chessBoard[i][curr_Y]->getName() != -1)
                        return true;
                }
            }
            else
            {
                for (int i = curr_X - 1; i > new_X; i--)
                {
                    if (chessBoard[i][curr_Y]->getName() != -1)
                        return true;
                }
            }
        }

        if (chessBoard[new_X][new_Y]->getName() != -1 && chessBoard[new_X][new_Y]->getColour() != colour)
            return false;
        else if (chessBoard[new_X][new_Y]->getName() != -1)
            return true;
        else
            return false;
    }

    void move_position(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        handleEnPesantFalse(chessBoard);
        if (is_first_move == true)
            is_first_move = false;
        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        Pieces *newPiece = chessBoard[new_X][new_Y];
        if (newPiece->getName() != -1)
            newPiece->Kill();
        chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
        position = new_position;
        chessBoard[curr_X][curr_Y] = new Blank(-1, 0, {-1, -1});
    }
};

class Bishop : public Pieces
{
public:
    Bishop(int name, bool colour, pair<int, int> position) : Pieces(name, colour, position) {}

    Bishop *clone() const override
    {
        Bishop *newCopy = new Bishop(name, colour, position);
        newCopy->alive = alive;
        newCopy->is_first_move = is_first_move;
        newCopy->is_last_square_move = is_last_square_move;

        return newCopy;
    }

    bool is_valid_move(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (this->alive == false || check_boundry(new_position))
            return false;

        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        if ((abs(new_X - curr_X) != abs(new_Y - curr_Y)) || (abs(new_X - curr_X) == 0))
            return false;
        else
        {
            if (check_obstacle(new_position, chessBoard))
                return false;
            else
            {
                move_position(new_position, chessBoard);
                return true;
            }
        }
    }

private:
    bool check_obstacle(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        int diff = abs(new_X - curr_X);

        if (new_X > curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (int i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X + i][curr_Y + i]->getName() != -1)
                        return true;
                }
            }
            else
            {
                for (int i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X + i][curr_Y - i]->getName() != -1)
                        return true;
                }
            }
        }
        else if (new_X < curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (int i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X - i][curr_Y + i]->getName() != -1)
                        return true;
                }
            }
            else
            {
                for (int i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X - i][curr_Y - i]->getName() != -1)
                        return true;
                }
            }
        }

        if (chessBoard[new_X][new_Y]->getName() != -1 && chessBoard[new_X][new_Y]->getColour() != chessBoard[curr_X][curr_Y]->getColour())
            return false;
        else if (chessBoard[new_X][new_Y]->getName() != -1)
            return true;
        else
            return false;
    }

    void move_position(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        handleEnPesantFalse(chessBoard);
        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        Pieces *newPiece = chessBoard[new_X][new_Y];
        if (newPiece->getName() != -1)
            newPiece->Kill();

        chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
        position = new_position;
        chessBoard[curr_X][curr_Y] = new Blank(-1, 0, {-1, -1});
    }
};

class Knight : public Pieces
{
public:
    Knight(int name, bool colour, pair<int, int> position) : Pieces(name, colour, position) {}

    Knight *clone() const override
    {
        Knight *newCopy = new Knight(name, colour, position);
        newCopy->alive = alive;
        newCopy->is_first_move = is_first_move;
        newCopy->is_last_square_move = is_last_square_move;

        return newCopy;
    }

    bool is_valid_move(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (this->alive == false || check_boundry(new_position))
        {
            return false;
        }

        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        int row[8] = {2, 2, -2, -2, 1, 1, -1, -1};
        int col[8] = {1, -1, -1, 1, 2, -2, 2, -2};

        for (int i = 0; i < 8; i++)
        {
            if (curr_X + row[i] == new_X && curr_Y + col[i] == new_Y)
            {
                if (check_obstacle(new_position, chessBoard))
                    return false;
                else
                {
                    move_position(new_position, chessBoard);
                    return true;
                }
            }
        }
        return false;
    }

private:
    bool check_obstacle(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        if (chessBoard[new_X][new_Y]->getName() != -1 && chessBoard[new_X][new_Y]->getColour() != chessBoard[curr_X][curr_Y]->getColour())
            return false;
        else if (chessBoard[new_X][new_Y]->getName() != -1)
            return true;
        else
            return false;
    }

    void move_position(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        handleEnPesantFalse(chessBoard);
        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        Pieces *newPiece = chessBoard[new_X][new_Y];
        if (newPiece->getName() != -1)
            newPiece->Kill();

        chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
        position = new_position;
        chessBoard[curr_X][curr_Y] = new Blank(-1, 0, {-1, -1});
    }
};

class Queen : public Pieces
{
public:
    Queen(int name, bool colour, pair<int, int> position) : Pieces(name, colour, position) {}

    Queen *clone() const override
    {
        Queen *newCopy = new Queen(name, colour, position);
        newCopy->alive = alive;
        newCopy->is_first_move = is_first_move;
        newCopy->is_last_square_move = is_last_square_move;

        return newCopy;
    }

    bool is_valid_move(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (this->alive == false || check_boundry(new_position))
            return false;

        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        if (new_X == curr_X && new_Y == curr_Y) // not a same place
            return false;
        else if ((abs(new_X - curr_X) == abs(new_Y - curr_Y)) || (new_X == curr_X || new_Y == curr_Y)) // bishop or Rook
        {
            if (check_obstacle(new_position, chessBoard))
                return false;
            else
            {
                move_position(new_position, chessBoard);
                return true;
            }
        }
        else
            return false;
    }

private:
    bool check_obstacle(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        int diff = abs(new_X - curr_X);

        if (new_X == curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (int i = curr_Y + 1; i < new_Y; i++)
                {
                    if (chessBoard[curr_X][i]->getName() != -1)
                        return true;
                }
            }
            else
            {
                for (int i = curr_Y - 1; i > new_Y; i--)
                {
                    if (chessBoard[curr_X][i]->getName() != -1)
                        return true;
                }
            }
        }
        else if (new_Y == curr_Y)
        {
            if (new_X > curr_X)
            {
                for (int i = curr_X + 1; i < new_X; i++)
                {
                    if (chessBoard[i][curr_Y]->getName() != -1)
                        return true;
                }
            }
            else
            {
                for (int i = curr_X - 1; i > new_X; i--)
                {
                    if (chessBoard[i][curr_Y]->getName() != -1)
                        return true;
                }
            }
        }
        else if (new_X > curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (int i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X + i][curr_Y + i]->getName() != -1)
                        return true;
                }
            }
            else
            {
                for (int i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X + i][curr_Y - i]->getName() != -1)
                        return true;
                }
            }
        }
        else if (new_X < curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (int i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X - i][curr_Y + i]->getName() != -1)
                        return true;
                }
            }
            else
            {
                for (int i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X - i][curr_Y - i]->getName() != -1)
                        return true;
                }
            }
        }

        if (chessBoard[new_X][new_Y]->getName() != -1 && chessBoard[new_X][new_Y]->getColour() != chessBoard[curr_X][curr_Y]->getColour())
            return false;
        else if (chessBoard[new_X][new_Y]->getName() != -1)
            return true;
        else
            return false;
    }

    void move_position(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        handleEnPesantFalse(chessBoard);
        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        Pieces *newPiece = chessBoard[new_X][new_Y];
        if (newPiece->getName() != -1)
            newPiece->Kill();

        chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
        position = new_position;
        chessBoard[curr_X][curr_Y] = new Blank(-1, 0, {-1, -1});
    }
};

class Pawn : public Pieces
{
private:
    bool isCopy = false;

public:
    Pawn(int name, bool colour, pair<int, int> position) : Pieces(name, colour, position)
    {
        is_last_square_move = false;
    }

    Pawn *clone() const override
    {
        Pawn *newCopy = new Pawn(name, colour, position);
        newCopy->alive = alive;
        newCopy->is_first_move = is_first_move;
        newCopy->is_last_square_move = is_last_square_move;
        newCopy->isCopy = true;
        return newCopy;
    }

    bool is_valid_move(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (check_boundry(new_position))
        {
            return false;
        }

        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        // for white
        if (!colour)
        {
            if (curr_Y == new_Y)
            {
                if (check_obstacle(new_position, chessBoard))
                    return false;
                if (is_first_move && new_X - curr_X == 2)
                {
                    handleEnPesantFalse(chessBoard);
                    is_first_move = false;

                    // for em pesant
                    pair<int, int> side_position = {new_X, new_Y + 1};

                    if (!check_boundry(side_position))
                    {
                        auto [emPesOpp, emPesOppColour] = which_piece(chessBoard[new_X][new_Y + 1]->getName());
                        if (emPesOpp == "Pawn" && emPesOppColour != colour)
                        {
                            is_last_square_move = true;
                        }
                    }

                    side_position = {new_X, new_Y - 1};
                    if (!check_boundry(side_position))
                    {
                        auto [emPesOpp, emPesOppColour] = which_piece(chessBoard[new_X][new_Y - 1]->getName());
                        if (emPesOpp == "Pawn" && emPesOppColour != colour)
                        {
                            is_last_square_move = true;
                        }
                    }
                    move_position(new_position, chessBoard);
                    return true;
                }
                else if (new_X - curr_X == 1)
                {
                    handleEnPesantFalse(chessBoard);
                    if (is_first_move)
                        is_first_move = false;
                    is_last_square_move = false;
                    move_position(new_position, chessBoard);
                    return true;
                }
                else
                    return false;
            }
            else if (abs(new_Y - curr_Y) == 1)
            {
                if ((new_X - curr_X) != 1)
                    return false;
                else if (check_obstacle(new_position, chessBoard))
                {
                    handleEnPesantFalse(chessBoard);
                    move_position(new_position, chessBoard);
                    return true;
                }
                else
                {
                    // must be En passant
                    auto [emPesPeice, emPesColour] = which_piece(chessBoard[curr_X][new_Y]->getName());
                    if (emPesPeice == "Pawn" && emPesColour != colour && chessBoard[curr_X][new_Y]->isLastSquareMove() == true)
                    {
                        Pieces *newPiece = chessBoard[curr_X][new_Y];
                        newPiece->Kill();
                        chessBoard[curr_X][new_Y] = new Blank(-1, 0, {-1, -1});
                        handleEnPesantFalse(chessBoard);
                        move_position(new_position, chessBoard);
                        return true;
                    }
                    else
                        return false;
                }
            }
            else
                return false;
        }
        else // for black
        {
            if (curr_Y == new_Y)
            {
                if (check_obstacle(new_position, chessBoard))
                    return false;
                if (is_first_move && curr_X - new_X == 2)
                {
                    handleEnPesantFalse(chessBoard);
                    is_first_move = false;

                    // for en pesant
                    pair<int, int> side_position = {new_X, new_Y + 1};

                    if (!check_boundry(side_position))
                    {
                        auto [emPesOpp, emPesOppColour] = which_piece(chessBoard[new_X][new_Y + 1]->getName());
                        if (emPesOpp == "Pawn" && emPesOppColour != colour)
                        {
                            is_last_square_move = true;
                        }
                    }

                    side_position = {new_X, new_Y - 1};
                    if (!check_boundry(side_position))
                    {
                        auto [emPesOpp, emPesOppColour] = which_piece(chessBoard[new_X][new_Y - 1]->getName());
                        if (emPesOpp == "Pawn" && emPesOppColour != colour)
                        {
                            is_last_square_move = true;
                        }
                    }
                    move_position(new_position, chessBoard);
                    return true;
                }
                else if (curr_X - new_X == 1)
                {
                    handleEnPesantFalse(chessBoard);
                    if (is_first_move)
                        is_first_move = false;
                    is_last_square_move = false;
                    move_position(new_position, chessBoard);
                    return true;
                }
                else
                    return false;
            }
            else if (abs(new_Y - curr_Y) == 1)
            {
                if ((curr_X - new_X) != 1)
                    return false;
                else if (check_obstacle(new_position, chessBoard))
                {
                    handleEnPesantFalse(chessBoard);
                    move_position(new_position, chessBoard);
                    return true;
                }
                else
                {
                    // must be En passant
                    auto [emPesPeice, emPesColour] = which_piece(chessBoard[curr_X][new_Y]->getName());
                    if (emPesPeice == "Pawn" && emPesColour != colour && chessBoard[curr_X][new_Y]->isLastSquareMove() == true)
                    {
                        Pieces *newPiece = chessBoard[curr_X][new_Y];
                        newPiece->Kill();
                        chessBoard[curr_X][new_Y] = new Blank(-1, 0, {-1, -1});

                        handleEnPesantFalse(chessBoard);
                        move_position(new_position, chessBoard);
                        return true;
                    }
                    else
                        return false;
                }
            }
            else
                return false;
        }
    }

private:
    bool check_obstacle(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        // for white
        if (!colour)
        {
            if (new_Y == curr_Y)
            {
                if ((new_X - curr_X == 2) && (chessBoard[new_X][new_Y]->getName() != -1 || chessBoard[new_X - 1][new_Y]->getName() != -1))
                    return true;
                else if (new_X - curr_X == 1 && chessBoard[new_X][new_Y]->getName() != -1)
                {
                    return true;
                }
                else
                    return false;
            }
            else if (abs(new_Y - curr_Y) == 1)
            {
                if (chessBoard[new_X][new_Y]->getName() != -1 && chessBoard[new_X][new_Y]->getColour() != chessBoard[curr_X][curr_Y]->getColour())
                    return true;
                else
                    return false;
            }
        }
        else // for black
        {
            if (new_Y == curr_Y)
            {
                if (((curr_X - new_X) == 2) && (chessBoard[new_X][new_Y]->getName() != -1 || chessBoard[new_X + 1][new_Y]->getName() != -1))
                    return true;
                else if (curr_X - new_X == 1 && chessBoard[new_X][new_Y]->getName() != -1)
                    return true;
                else
                    return false;
            }
            else if (abs(new_Y - curr_Y) == 1)
            {
                if (chessBoard[new_X][new_Y]->getName() != -1 && chessBoard[new_X][new_Y]->getColour() != chessBoard[curr_X][curr_Y]->getColour())
                    return true;
                else
                    return false;
            }
        }

        // just to avoid the warning this is not returning false for any reason and we will never reach here..
        return false;
    }

    void move_position(pair<int, int> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        int curr_X = position.first;
        int curr_Y = position.second;

        int new_X = new_position.first;
        int new_Y = new_position.second;

        Pieces *newPiece = chessBoard[new_X][new_Y];

        if (newPiece->getName() != -1)
            newPiece->Kill();

        chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
        position = new_position;
        chessBoard[curr_X][curr_Y] = new Blank(-1, 0, {-1, -1});

        while (!isCopy && ((colour == 0 && new_X == 7) || (colour == 1 && new_X == 0)))
        {
            cout << "Hurray We have some bonus for you Enter the options which ever you want to upgrade this pawn to : " << endl;
            cout << "Queen Rook Knight Bishop None" << endl;
            cout << "enter the name : ";
            string s;
            cin >> s;
            if (s == "Queen")
            {
                int new_name = (colour == 0) ? 14 : 31;
                chessBoard[new_X][new_Y]->Kill();
                chessBoard[new_X][new_Y] = new Queen(new_name, colour, position);
            }
            else if (s == "Knight")
            {
                int new_name = (colour == 0) ? 10 : 26;
                chessBoard[new_X][new_Y]->Kill();
                chessBoard[new_X][new_Y] = new Knight(new_name, colour, position);
            }
            else if (s == "Bishop")
            {
                int new_name = (colour == 0) ? 12 : 28;
                chessBoard[new_X][new_Y]->Kill();
                chessBoard[new_X][new_Y] = new Bishop(new_name, colour, position);
            }
            else if (s == "Rook")
            {
                int new_name = (colour == 0) ? 8 : 24;
                chessBoard[new_X][new_Y]->Kill();
                chessBoard[new_X][new_Y] = new Rook(new_name, colour, position);
            }
            else if (s == "None")
            {
                // do nothing
            }
            else
            {
                cout << "invalid choice" << endl;
                continue;
            }
            break;
        }
    }
};

class Chess
{
private:
    vector<vector<Pieces *>> chessBoard;
    King *white_king;
    King *black_king;

public:
    Chess()
    {
        chessBoard.resize(8, vector<Pieces *>(8));

        int chessValues[8][8] = {
            {8, 10, 12, 14, 15, 13, 11, 9},   // First row: White's main pieces (Rook, Knight, Bishop, Queen, King, etc.)
            {0, 1, 2, 3, 4, 5, 6, 7},         // Second row: White's pawns
            {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
            {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
            {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
            {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
            {16, 17, 18, 19, 20, 21, 22, 23}, // Seventh row: Black's pawns
            {24, 26, 28, 31, 30, 29, 27, 25}  // Eighth row: Black's main pieces (Rook, Knight, Bishop, Queen, King, etc.)
        };

        for (int i = 0; i < Size; i++)
        {
            for (int j = 0; j < Size; j++)
            {
                auto a = Pieces::which_piece(chessValues[i][j]);
                string Name = a.first;
                bool colour = a.second;
                pair<int, int> position = make_pair(i, j);

                if (Name == "blank")
                {
                    chessBoard[i][j] = new Blank(chessValues[i][j], colour, position);
                }
                else if (Name == "Pawn")
                {
                    chessBoard[i][j] = new Pawn(chessValues[i][j], colour, position);
                }
                if (Name == "King")
                {
                    if (colour)
                    {
                        black_king = new King(chessValues[i][j], colour, position);
                    }
                    else
                    {
                        white_king = new King(chessValues[i][j], colour, position);
                    }
                    chessBoard[i][j] = (colour == 0) ? white_king : black_king;
                }
                else if (Name == "Queen")
                {
                    chessBoard[i][j] = new Queen(chessValues[i][j], colour, position);
                }
                else if (Name == "Bishop")
                {
                    chessBoard[i][j] = new Bishop(chessValues[i][j], colour, position);
                }
                else if (Name == "Knight")
                {
                    chessBoard[i][j] = new Knight(chessValues[i][j], colour, position);
                }
                else if (Name == "Rook")
                {
                    chessBoard[i][j] = new Rook(chessValues[i][j], colour, position);
                }
            }
        }
    }

private:
    // If we are ending with these cases then the game could not end.
    bool insufficient_material()
    {
        int white_pieces = 0, black_pieces = 0;
        bool white_has_bishop = false, white_has_knight = false;
        bool black_has_bishop = false, black_has_knight = false;
        bool white_bishop_color = false, black_bishop_color = false;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                Pieces *piece = chessBoard[i][j];
                string pieceName = Pieces::which_piece(piece->getName()).first;

                if (piece->getName() == -1)
                    continue; // Empty square, skip

                if (Pieces::which_piece(piece->getName()).first == "King")
                {
                    continue; // Ignore kings
                }

                if (piece->getColour() == 0)
                { // White pieces
                    white_pieces++;
                    if (pieceName == "Bishop")
                    {
                        white_has_bishop = true;
                        white_bishop_color = (i + j) % 2 == 0; // track bishop color (white/black square)
                    }
                    else if (Pieces::which_piece(piece->getName()).first == "Knight")
                    {
                        white_has_knight = true;
                    }
                }
                else
                { // Black pieces
                    black_pieces++;
                    if (pieceName == "Bishop")
                    {
                        black_has_bishop = true;
                        black_bishop_color = (i + j) % 2 == 0; // track bishop color (white/black square)
                    }
                    else if (pieceName == "Knight")
                    {
                        black_has_knight = true;
                    }
                }
            }
        }

        // Case 1: King vs King
        if (white_pieces == 0 && black_pieces == 0)
            return true;

        // Case 2: King and Bishop vs King
        else if (white_pieces == 1 && white_has_bishop && black_pieces == 0)
            return true;
        else if (black_pieces == 1 && black_has_bishop && white_pieces == 0)
            return true;

        // Case 3: King and Knight vs King
        else if (white_pieces == 1 && white_has_knight && black_pieces == 0)
            return true;
        else if (black_pieces == 1 && black_has_knight && white_pieces == 0)
            return true;

        // Case 4: King and Bishop vs King and Bishop (same color)
        else if (white_pieces == 1 && black_pieces == 1 && white_has_bishop && black_has_bishop && white_bishop_color == black_bishop_color)
        {
            return true;
        }

        // No insufficient material scenario
        else
            return false;
    }

public:
    void print_chessboard()
    {
        cout << "Current Chessboard State:" << endl;
        cout << "  +-------------------------------------------------------------------------------------------------------------------------------------------------------+" << endl;

        for (int x = 7; x >= 0; x--)
        {
            cout << x << " |";

            for (int y = 0; y < 8; y++)
            {
                Pieces *piece = chessBoard[x][y];
                cout << "[" << x << " " << y << "]";
                if (piece->getName() == -1)
                {
                    cout << "             |";
                }
                else
                {
                    // Determine the color and type of piece
                    string color = (piece->getColour() == 0) ? "White" : "Black";
                    string pieceType;

                    // Map the name to the piece type
                    switch (piece->getName())
                    {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                        pieceType = "Pawn  |";
                        break;
                    case 8:
                    case 9:
                        pieceType = "Rook  |";
                        break;
                    case 10:
                    case 11:
                        pieceType = "Knight|";
                        break;
                    case 12:
                    case 13:
                        pieceType = "Bishop|";
                        break;
                    case 14:
                        pieceType = "Queen |";
                        break;
                    case 15:
                        pieceType = "King  |";
                        break;
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                        pieceType = "Pawn  |";
                        break;
                    case 24:
                    case 25:
                        pieceType = "Rook  |";
                        break;
                    case 26:
                    case 27:
                        pieceType = "Knight|";
                        break;
                    case 28:
                    case 29:
                        pieceType = "Bishop|";
                        break;
                    case 30:
                        pieceType = "King  |";
                        break;
                    case 31:
                        pieceType = "Queen |";
                        break;
                    default:
                        pieceType = "Unknown|";
                        break;
                    }

                    cout << " " << color << " " << pieceType; // Print the piece with color
                }
            }
            cout << endl;
            cout << "  +-------------------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
        }

        cout << "            0                  1                   2                 3                  4                  5                  6                  7" << endl; // Column labels
        cout << endl;
    }

    void readSquare(const string &prompt, int &x, int &y)
    {
        while (true)
        {
            cout << prompt;
            if (cin >> x >> y)
            {
                return;
            }
            else
            {
                // input failed (e.g. user typed a letter)
                cout << "Invalid input. Please enter two numbers between 0 and 7.\n";
                cin.clear(); // clear the failbit
            }
            // discard the rest of the line so we can retry cleanly
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    bool color = 0;

    bool turns()
    {
        cout << "Turn " << (color == 0 ? "white" : "black") << endl;

        // check stalemate
        if ((color == 0 && white_king->is_stalemate(chessBoard, color)) || (color == 1 && black_king->is_stalemate(chessBoard, color)))
        {
            cout << "Draw due to stalemate.....!!";
            return false;
        }

        // check checkmate
        if ((color == 0 && white_king->is_check(chessBoard)) || (color == 1 && black_king->is_check(chessBoard)))
        {
            if ((color == 0 && white_king->is_checkmate(chessBoard)) || (color == 1 && black_king->is_checkmate(chessBoard)))
            {
                string ans = (color == 1) ? "White" : "Black";
                cout << "Check Mate...!! " << ans << " is a winner" << endl;
                return false;
            }
            cout << "There is a check" << endl;
        }

        // check insufficient material
        if (insufficient_material())
        {
            cout << "Draw due to insuffiecient material...!!" << endl;
            return false;
        }

        int curr_X, curr_Y;

        int new_X, new_Y;

        readSquare("Please enter the valid piece coordinates (x y): ", curr_X, curr_Y);
        readSquare("Please enter the valid destination coordinates (x y): ", new_X, new_Y);

        if (Pieces::check_boundry({curr_X, curr_Y}) || chessBoard[curr_X][curr_Y]->getName() == -1 || chessBoard[curr_X][curr_Y]->getColour() != color)
        {
            cout << "Invalid starting piece or position." << endl;
            return true;
        }

        pair<int, int> new_position = make_pair(new_X, new_Y);
        if (Pieces::check_boundry(new_position))
        {
            cout << "Invalid destination position." << endl;
            return true;
        }

        vector<vector<Pieces *>> copyBoard = Pieces::deepCopyBoard(chessBoard);

        King *white_copy = nullptr;
        King *black_copy = nullptr;

        // Scan every square
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                Pieces *p = copyBoard[i][j];
                if (auto k = dynamic_cast<King *>(p))
                {
                    // We found a King; check its color
                    if (k->getColour() == 0)
                        white_copy = k;
                    else /* k->getColour() == 1 */
                        black_copy = k;
                }
            }
        }

        Pieces *p = copyBoard[curr_X][curr_Y];
        Pieces *new_piece = copyBoard[new_X][new_Y];

        if (!(p->is_valid_move(new_position, copyBoard)))
        {
            print_chessboard();
            cout << "Invalid destination position." << endl;
            Pieces::deleteBoard(copyBoard);
            return true;
        }

        if ((!color && white_copy->is_check(copyBoard)) || (color && black_copy->is_check(copyBoard)))
        {
            cout << "Invalid move due to introducing check." << endl;
            Pieces::deleteBoard(copyBoard);
            return true;
        }
        // Actual move

        cout << "Moving piece from (" << curr_X << "," << curr_Y
             << ") to (" << new_X << "," << new_Y << ").\n";
        p = chessBoard[curr_X][curr_Y];

        new_piece = chessBoard[new_X][new_Y];
        p->is_valid_move(new_position, chessBoard);

        print_chessboard();
        color = !color;
        Pieces::deleteBoard(copyBoard);
        return true;
    }

    ~Chess()
    {
        Pieces::deleteBoard(chessBoard);
        delete white_king;
        delete black_king;
    }
};

int main()
{
    // Programmed by By Parth Sorathiya;
    Chess c;
    c.print_chessboard();
    while (c.turns())
        ;
}
