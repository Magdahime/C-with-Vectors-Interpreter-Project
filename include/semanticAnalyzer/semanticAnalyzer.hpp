#include <cstdint>
#include <stack>
#include <unordered_map>



// VISITOR CLASS
class SemanticAnalyzer {
 public:
  enum class Type {
    Integer,
    String,
    Double,
    Matrix
  }

  struct VariableInfo {
    SemanticAnalyzer::Type type;
    TokenVariant value;
  };

  struct FunctionInfo {
    Type returnType;
    std::vector<Type> arguments;
  }

  void enterBlock();
  void closeBlock();
  void enterVariable();
  void enterFunction();
  std::optional<VariableInfo> searchVariable(std::string identifier);
  std::optional<FunctionInfo> searchFunction(std::string identifier);


 private:
  using variablesMap = std::unordered_map<std::string, VariableInfo>;
  using functionsMap = std::unordered_map<std::string, FunctionInfo>;
  std::stack<std::pair<uint64_t, variablesMap>> scopeStack;

};  // namespace SemanticAnalyzer