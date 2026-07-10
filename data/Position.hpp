#ifndef POSITION_HPP
#define POSITION_HPP

struct Position {
    int row;
    int col;
    
    Position(int r = 1, int c = 1) : row(r), col(c) {}
};

#endif // POSITION_HPP
