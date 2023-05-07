#pragma once

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

class CounterBite {
  public:
    CounterBite() : totalBite_(0){};

    void AddCounter(const std::string &str) { totalBite_ += str.size(); };

    size_t totalBite_;
};

class IOperation;

using IOperationUP = std::unique_ptr<IOperation>;

class IHandleEndOfInput {
  public:
    virtual void HandleEndOfInput() = 0;
    virtual ~IHandleEndOfInput() = default;
};

class IProcessLine {
  public:
    virtual void ProcessLine(const std::string &str) = 0;
    virtual ~IProcessLine() = default;
};

class ISetNextOperation {
  public:
    virtual void SetNextOperation(IOperation *operation) = 0;
    virtual ~ISetNextOperation() = default;
};

class IOperation : public IHandleEndOfInput,
                   public ISetNextOperation,
                   public IProcessLine {};

class OperationWithNext : public ISetNextOperation {
  public:
    void SetNextOperation(IOperation *operation) override;

  protected:
    IOperationUP next_;
};

class OperationWithArg {
  protected:
    OperationWithArg(){};
    OperationWithArg(const std::string &arg) : arg_(arg){};

    std::string arg_;
};

class OperationWithOut {
  public:
    OperationWithOut(std::ostream &out) : out_(out){};

  protected:
    std::ostream &out_;
};

class BaseOperation : public OperationWithNext,
                      public OperationWithArg,
                      public OperationWithOut {
  protected:
    BaseOperation(const std::string &arg, std::ostream &out)
        : OperationWithOut(out), OperationWithArg(arg){};
};

class EchoOperation : public IOperation, public BaseOperation {
  public:
    EchoOperation(const std::string &arg, std::ostream &out)
        : BaseOperation(arg, out){};

    void HandleEndOfInput() override;

    void SetNextOperation(IOperation *operation) override {
        OperationWithNext::SetNextOperation(operation);
    };

    void ProcessLine(const std::string &str) override;

    ~EchoOperation() override = default;
};

class CatOperation : public IOperation, public BaseOperation {
  public:
    CatOperation(const std::string &arg, std::ostream &out)
        : BaseOperation(arg, out){};

    void HandleEndOfInput() override;

    void SetNextOperation(IOperation *operation) override {
        OperationWithNext::SetNextOperation(operation);
    };

    void ProcessLine(const std::string &str) override;

    ~CatOperation() override = default;
};

// WCOperation can throw std::runtime_error
class WCOperation : public IOperation, public BaseOperation {
  public:
    WCOperation(const std::string &arg, std::ostream &out);

    void HandleEndOfInput() override;

    void SetNextOperation(IOperation *operation) override {
        OperationWithNext::SetNextOperation(operation);
    };

    void ProcessLine(const std::string &str) override;

    ~WCOperation() override = default;

  protected:
    ::CounterBite counter_;
};
