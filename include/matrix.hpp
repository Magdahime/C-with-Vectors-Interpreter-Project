#include <valarray>
#include <variant>
// TODO IMPLEMENT THIS
class Matrix{
    public:
    private:
    unsigned int columns;
    unsigned int rows;
    std::valarray<std::variant<double, int>> values;
};