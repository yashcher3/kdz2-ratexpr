// #pragma once
//
// #include <iosfwd>
// #include <memory>
//
// #include <ratexpr/fraction.hpp>
//
// namespace ratexpr {
//
//     class Expression {
//     public:
//         virtual ~Expression() = default;
//         virtual Fraction evaluate() const = 0;
//         virtual void print(std::ostream& os) const = 0;
//     };
//
//     std::ostream& operator<<(std::ostream& os, const Expression& e);
//
//     class Const : public Expression {
//     public:
//         explicit Const(const Fraction& value);
//         Fraction evaluate() const override;
//         void print(std::ostream& os) const override;
//
//     private:
//         Fraction value_;
//     };
//
//     class Negative : public Expression {
//     public:
//         explicit Negative(std::unique_ptr<Expression> operand);
//         Fraction evaluate() const override;
//         void print(std::ostream& os) const override;
//
//     private:
//         std::unique_ptr<Expression> operand_;
//     };
//
//     class BinaryExpression : public Expression {
//     public:
//         BinaryExpression(std::unique_ptr<Expression> lhs,
//                          std::unique_ptr<Expression> rhs);
//         void print(std::ostream& os) const override;
//
//     protected:
//         virtual const char* symbol() const = 0;
//
//         std::unique_ptr<Expression> lhs_;
//         std::unique_ptr<Expression> rhs_;
//     };
//
//     class Add : public BinaryExpression {
//     public:
//         using BinaryExpression::BinaryExpression;
//         Fraction evaluate() const override;
//
//     protected:
//         const char* symbol() const override { return "+"; }
//     };
//
//     class Sub : public BinaryExpression {
//     public:
//         using BinaryExpression::BinaryExpression;
//         Fraction evaluate() const override;
//
//     protected:
//         const char* symbol() const override { return "-"; }
//     };
//
//     class Mul : public BinaryExpression {
//     public:
//         using BinaryExpression::BinaryExpression;
//         Fraction evaluate() const override;
//
//     protected:
//         const char* symbol() const override { return "*"; }
//     };
//
//     class Div : public BinaryExpression {
//     public:
//         using BinaryExpression::BinaryExpression;
//         Fraction evaluate() const override;
//
//     protected:
//         const char* symbol() const override { return "/"; }
//     };
//
// }

#pragma once

#include <cstddef>
#include <iosfwd>
#include <memory>

#include <ratexpr/fraction.hpp>

namespace ratexpr {

class Expression {
public:
    virtual ~Expression() = default;
    virtual Fraction evaluate() const = 0;
    virtual void print(std::ostream& os) const = 0;
};

std::ostream& operator<<(std::ostream& os, const Expression& e);

class Const : public Expression {
public:
    explicit Const(const Fraction& value);
    Fraction evaluate() const override;
    void print(std::ostream& os) const override;
    const Fraction& value() const { return value_; }

private:
    Fraction value_;
};

class Negative : public Expression {
public:
    explicit Negative(std::unique_ptr<Expression> operand);
    Fraction evaluate() const override;
    void print(std::ostream& os) const override;
    const Expression& operand() const { return *operand_; }

private:
    std::unique_ptr<Expression> operand_;
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(std::unique_ptr<Expression> lhs,
                     std::unique_ptr<Expression> rhs);
    void print(std::ostream& os) const override;
    const Expression& lhs() const { return *lhs_; }
    const Expression& rhs() const { return *rhs_; }

    virtual std::unique_ptr<Expression> with_operands(
        std::unique_ptr<Expression> left,
        std::unique_ptr<Expression> right) const = 0;

protected:
    virtual const char* symbol() const = 0;

    std::unique_ptr<Expression> lhs_;
    std::unique_ptr<Expression> rhs_;
};

class Add : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;
    Fraction evaluate() const override;
    std::unique_ptr<Expression> with_operands(
        std::unique_ptr<Expression> left,
        std::unique_ptr<Expression> right) const override;

protected:
    const char* symbol() const override { return "+"; }
};

class Sub : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;
    Fraction evaluate() const override;
    std::unique_ptr<Expression> with_operands(
        std::unique_ptr<Expression> left,
        std::unique_ptr<Expression> right) const override;

protected:
    const char* symbol() const override { return "-"; }
};

class Mul : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;
    Fraction evaluate() const override;
    std::unique_ptr<Expression> with_operands(
        std::unique_ptr<Expression> left,
        std::unique_ptr<Expression> right) const override;

protected:
    const char* symbol() const override { return "*"; }
};

class Div : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;
    Fraction evaluate() const override;
    std::unique_ptr<Expression> with_operands(
        std::unique_ptr<Expression> left,
        std::unique_ptr<Expression> right) const override;

protected:
    const char* symbol() const override { return "/"; }
};

std::size_t count_operations(const Expression& e);

std::unique_ptr<Expression> simplify(const Expression& e);

}
