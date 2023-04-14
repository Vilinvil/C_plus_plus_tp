#include <fstream>
#include <iostream>
#include <memory>
#include <string>

struct CounterBite {
    CounterBite() : flag_(false), totalBite_(0){};
    CounterBite(bool flag, int totalBite)
        : flag_(flag), totalBite_(totalBite){};

    void AddCounter(int sum) { totalBite_ += sum; };
    void SetFlag(bool flag) { flag_ = flag; };

    // if exist wc => all operation before flag = true
    bool flag_;

    // counterTotalBite = sum(all prev byte operation) + cur(sum byte)
    int totalBite_;
};

class IOperation;
class ICounter;

using IOperationUP = std::unique_ptr<IOperation>;

class IOperation {
  public:
    virtual void ProcessLine(const std::string &str) = 0;
    virtual void HandleEndOfInput() = 0;
    virtual void SetNextOperation(IOperationUP operation) = 0;
};

class ICounter {
  public:
    virtual int CountBite() = 0;
};

class EchoOperation : public IOperation, ICounter {
  public:
    EchoOperation()
        : counter_(CounterBite()), next_(nullptr), str_(""), out_(std::cout){};
    EchoOperation(IOperationUP &&next, std::string str, std::ostream out)
        : counter_(CounterBite()), str_(str), out_(out) {
        next_ = std::exchange(next, nullptr);
    }

    void ProcessLine(const std::string &str) override;
    void HandleEndOfInput() override;
    void SetNextOperation(IOperationUP operation) override;

    int CountBite() override { return counter_.totalBite_; };

  private:
    IOperationUP next_;

    std::string str_;

    CounterBite counter_;

    std::ostream &out_;
};

void EchoOperation::ProcessLine(const std::string &str) {
    str_ = str;
    if (counter_.flag_) {
        counter_.totalBite_ += str_.size();
    }
}

void EchoOperation::HandleEndOfInput() { out_ << str_; }

void EchoOperation::SetNextOperation(IOperationUP operation) {
    next_ = std::exchange(operation, nullptr);
}

class CatOperaion : public IOperation, ICounter {
  public:
    CatOperaion()
        : next_(nullptr), fileName_(""), counter_(CounterBite()),
          out_(std::cout){};
    CatOperaion(IOperationUP &&next, std::string fileName, std::ostream out)
        : fileName_(fileName), counter_(CounterBite()), out_(out) {
        next_ = std::exchange(next, nullptr);
    }

    void ProcessLine(const std::string &str) override;
    void HandleEndOfInput() override;
    void SetNextOperation(IOperationUP) override;

  private:
    IOperationUP next_;

    std::string fileName_;

    CounterBite counter_;

    std::ostream &out_;
};

void CatOperaion::ProcessLine(const std::string &str) {

    file = fopen(fileName_, "r");
}

class WCOperaion : public OperationCount {
  public:
    void ProcessLine(const std::string &str) override;
    void HandleEndOfInput() override;
    void SetNextOperation(IOperationUP) override;

  private:
    std::string strCounter_;
    CounterBite counter_;
};

int main(int argc, char *argv[]) {
    // std::string str("гг");
    // std::cout << str.size();
    return 0;
}