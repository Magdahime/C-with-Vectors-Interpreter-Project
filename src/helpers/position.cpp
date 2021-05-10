#include "helpers/position.hpp"

Position Position::nextChar() const
{
    return Position(this->absolutePosition +1, this->line, this->character + 1);
}
Position Position::nextLine() const 
{
    return Position(this->absolutePosition, this->line + 1, 0);
}