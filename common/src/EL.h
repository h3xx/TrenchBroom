/*
 Copyright (C) 2010-2016 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EL_h
#define EL_h

#include "Exceptions.h"
#include "Macros.h"
#include "StringUtils.h"
#include "SharedPointer.h"

#include <cstdlib>
#include <list>
#include <map>
#include <memory>
#include <vector>

namespace TrenchBroom {
    namespace EL {
        class Value;
        
        typedef bool BooleanType;
        typedef String StringType;
        typedef double NumberType;
        typedef std::vector<Value> ArrayType;
        typedef std::map<String, Value> MapType;
        typedef std::vector<long> RangeType;
        
        typedef enum {
            Type_Boolean,
            Type_String,
            Type_Number,
            Type_Array,
            Type_Map,
            Type_Range,
            Type_Null,
            Type_Undefined
        } ValueType;
        
        String typeName(ValueType type);
        ValueType typeForName(const String& type);
        
        class ELException : public ExceptionStream<ELException> {
        public:
            ELException() throw();
            ELException(const String& str) throw();
            ~ELException() throw();
        };
        
        class ConversionError : public ELException {
        public:
            ConversionError(const String& value, const ValueType from, const ValueType to) throw();
        };
        
        class DereferenceError : public ELException {
        public:
            DereferenceError(const String& value, const ValueType from, const ValueType to) throw();
        };
        
        class EvaluationError : public ELException {
        public:
            EvaluationError(const String& msg) throw();
        };
        
        class IndexError : public EvaluationError {
        public:
            IndexError(const Value& indexableValue, const Value& indexValue) throw();
            IndexError(const Value& indexableValue, size_t index) throw();
            IndexError(const Value& indexableValue, const String& key) throw();
        };
        
        class IndexOutOfBoundsError : public IndexError {
        public:
            IndexOutOfBoundsError(const Value& indexableValue, const Value& indexValue, size_t outOfBoundsIndex) throw();
            IndexOutOfBoundsError(const Value& indexableValue, const Value& indexValue, const String& outOfBoundsIndex) throw();
            IndexOutOfBoundsError(const Value& indexableValue, size_t index) throw();
            IndexOutOfBoundsError(const Value& indexableValue, const String& key) throw();
        };
        
        class ValueHolder {
        public:
            virtual ~ValueHolder();
            
            virtual ValueType type() const = 0;
            String describe() const;
            
            virtual const BooleanType& booleanValue() const;
            virtual const StringType&  stringValue()  const;
            virtual const NumberType&  numberValue()  const;
            virtual const ArrayType&   arrayValue()   const;
            virtual const MapType&     mapValue()     const;
            virtual const RangeType&   rangeValue()   const;

            virtual size_t length() const = 0;
            virtual ValueHolder* convertTo(ValueType toType) const = 0;
            
            virtual ValueHolder* clone() const = 0;
            
            virtual void appendToStream(std::ostream& str, bool multiline, const String& indent) const = 0;
        };
        
        class BooleanValueHolder : public ValueHolder {
        private:
            BooleanType m_value;
        public:
            BooleanValueHolder(const BooleanType& value);
            ValueType type() const;
            const BooleanType& booleanValue() const;
            size_t length() const;
            ValueHolder* convertTo(const ValueType toType) const;
            ValueHolder* clone() const;
            void appendToStream(std::ostream& str, bool multiline, const String& indent) const;
        };
        
        class StringValueHolder : public ValueHolder {
        private:
            StringType m_value;
        public:
            StringValueHolder(const StringType& value);
            ValueType type() const;
            const StringType& stringValue() const;
            size_t length() const;
            ValueHolder* convertTo(const ValueType toType) const;
            ValueHolder* clone() const;
            void appendToStream(std::ostream& str, bool multiline, const String& indent) const;
        };
        
        class NumberValueHolder : public ValueHolder {
        private:
            NumberType m_value;
        public:
            NumberValueHolder(const NumberType& value);
            ValueType type() const;
            const NumberType& numberValue() const;
            size_t length() const;
            ValueHolder* convertTo(const ValueType toType) const;
            ValueHolder* clone() const;
            void appendToStream(std::ostream& str, bool multiline, const String& indent) const;
        };
        
        class ArrayValueHolder : public ValueHolder {
        private:
            ArrayType m_value;
        public:
            ArrayValueHolder(const ArrayType& value);
            ValueType type() const;
            const ArrayType& arrayValue() const;
            size_t length() const;
            ValueHolder* convertTo(const ValueType toType) const;
            ValueHolder* clone() const;
            void appendToStream(std::ostream& str, bool multiline, const String& indent) const;
        };

        class MapValueHolder : public ValueHolder {
        private:
            MapType m_value;
        public:
            MapValueHolder(const MapType& value);
            ValueType type() const;
            const MapType& mapValue() const;
            size_t length() const;
            ValueHolder* convertTo(const ValueType toType) const;
            ValueHolder* clone() const;
            void appendToStream(std::ostream& str, bool multiline, const String& indent) const;
        };
        
        class RangeValueHolder : public ValueHolder {
        private:
            RangeType m_value;
        public:
            RangeValueHolder(const RangeType& value);
            ValueType type() const;
            const RangeType& rangeValue() const;
            size_t length() const;
            ValueHolder* convertTo(const ValueType toType) const;
            ValueHolder* clone() const;
            void appendToStream(std::ostream& str, bool multiline, const String& indent) const;
        };

        class NullValueHolder : public ValueHolder {
        public:
            ValueType type() const;
            const StringType& stringValue() const;
            const BooleanType& booleanValue() const;
            const NumberType& numberValue() const;
            const ArrayType& arrayValue() const;
            const MapType& mapValue() const;
            size_t length() const;
            ValueHolder* convertTo(const ValueType toType) const;
            ValueHolder* clone() const;
            void appendToStream(std::ostream& str, bool multiline, const String& indent) const;
        };
        
        class UndefinedValueHolder : public ValueHolder {
        public:
            ValueType type() const;
            size_t length() const;
            ValueHolder* convertTo(const ValueType toType) const;
            ValueHolder* clone() const;
            void appendToStream(std::ostream& str, bool multiline, const String& indent) const;
        };
        
        class Value {
        public:
            static const Value Null;
            static const Value Undefined;
            typedef std::set<Value> Set;
        private:
            typedef std::vector<size_t> IndexList;
            typedef std::tr1::shared_ptr<ValueHolder> ValuePtr;
            ValuePtr m_value;
            size_t m_line;
            size_t m_column;
        private:
            Value(ValueHolder* holder, size_t line, size_t column);
        public:
            Value(const BooleanType& value, size_t line, size_t column);
            explicit Value(const BooleanType& value);
            
            Value(const StringType& value, size_t line, size_t column);
            explicit Value(const StringType& value);
            
            Value(const char* value, size_t line, size_t column);
            explicit Value(const char* value);
            
            Value(const NumberType& value, size_t line, size_t column);
            explicit Value(const NumberType& value);
            
            Value(int value, size_t line, size_t column);
            explicit Value(int value);
            
            Value(long value, size_t line, size_t column);
            explicit Value(long value);
            
            Value(size_t value, size_t line, size_t column);
            explicit Value(size_t value);
            
            Value(const ArrayType& value, size_t line, size_t column);
            explicit Value(const ArrayType& value);
            
            template <typename T>
            Value(const std::vector<T>& value, size_t line, size_t column) :
            m_value(new ArrayValueHolder(makeArray(value))),
            m_line(line),
            m_column(column){}
            
            template <typename T>
            explicit Value(const std::vector<T>& value) :
            m_value(new ArrayValueHolder(makeArray(value))),
            m_line(0),
            m_column(0) {}
            
            Value(const MapType& value, size_t line, size_t column);
            explicit Value(const MapType& value);

            template <typename T, typename C>
            Value(const std::map<String, T, C>& value, size_t line, size_t column) :
            m_value(new MapValueHolder(makeMap(value))),
            m_line(line),
            m_column(column) {}
            
            template <typename T, typename C>
            explicit Value(const std::map<String, T, C>& value) :
            m_value(new MapValueHolder(makeMap(value))),
            m_line(0),
            m_column(0) {}
            
            Value(const RangeType& value, size_t line, size_t column);
            explicit Value(const RangeType& value);

            Value(const Value& other, size_t line, size_t column);
            Value(const Value& other);

            Value();
        private:
            template <typename T>
            ArrayType makeArray(const std::vector<T>& value) {
                ArrayType result;
                result.reserve(value.size());
                typename std::vector<T>::const_iterator it, end;
                for (it = value.begin(), end = value.end(); it != end; ++it)
                    result.push_back(EL::Value(*it));
                return result;
            }
            
            template <typename T, typename C>
            MapType makeMap(const std::map<String, T, C>& value) {
                MapType result;
                typename std::map<String, T, C>::const_iterator it, end;
                for (it = value.begin(), end = value.end(); it != end; ++it)
                    result.insert(std::make_pair(it->first, EL::Value(it->second)));
                return result;
            }
        public:
            ValueType type() const;
            String typeName() const;
            String describe() const;
            
            size_t line() const;
            size_t column() const;
            
            const StringType& stringValue() const;
            const BooleanType& booleanValue() const;
            const NumberType& numberValue() const;
            const ArrayType& arrayValue() const;
            const MapType& mapValue() const;
            const RangeType& rangeValue() const;
            bool null() const;
            
            const StringList asStringList() const;
            const StringSet asStringSet() const;
            
            size_t length() const;
            Value convertTo(ValueType toType) const;

            void appendToStream(std::ostream& str, bool multiline = true, const String& indent = "") const;
            friend std::ostream& operator<<(std::ostream& stream, const Value& value);
            
            bool contains(const Value& indexValue) const;
            bool contains(size_t index) const;
            bool contains(const String& key) const;
            StringSet keys() const;
            
            Value operator[](const Value& indexValue) const;
            Value operator[](size_t index) const;
            Value operator[](int index) const;
            Value operator[](const String& key) const;
            Value operator[](const char* key) const;
        private:
            IndexList computeIndexArray(const Value& indexValue, size_t indexableSize) const;
            void computeIndexArray(const Value& indexValue, size_t indexableSize, IndexList& result) const;
            size_t computeIndex(const Value& indexValue, size_t indexableSize) const;
            size_t computeIndex(long index, size_t indexableSize) const;
        public:
            Value operator+() const;
            Value operator-() const;
            
            friend Value operator+(const Value& lhs, const Value& rhs);
            friend Value operator-(const Value& lhs, const Value& rhs);
            friend Value operator*(const Value& lhs, const Value& rhs);
            friend Value operator/(const Value& lhs, const Value& rhs);
            friend Value operator%(const Value& lhs, const Value& rhs);

            operator bool() const;
            Value operator!() const;
            
            friend bool operator==(const Value& lhs, const Value& rhs);
            friend bool operator!=(const Value& lhs, const Value& rhs);
            friend bool operator<(const Value& lhs, const Value& rhs);
            friend bool operator<=(const Value& lhs, const Value& rhs);
            friend bool operator>(const Value& lhs, const Value& rhs);
            friend bool operator>=(const Value& lhs, const Value& rhs);
        private:
            friend int compare(const Value& lhs, const Value& rhs);
            friend int compareAsBooleans(const Value& lhs, const Value& rhs);
            friend int compareAsNumbers(const Value& lhs, const Value& rhs);
        };
        
        class VariableStore {
        public:
            virtual ~VariableStore();
            
            VariableStore* clone() const;
            Value value(const String& name) const;
            const StringSet names() const;
            void declare(const String& name, const Value& value = Value::Undefined);
            void assign(const String& name, const Value& value);
        private:
            virtual VariableStore* doClone() const = 0;
            virtual Value doGetValue(const String& name) const = 0;
            virtual StringSet doGetNames() const = 0;
            virtual void doDeclare(const String& name, const Value& value) = 0;
            virtual void doAssign(const String& name, const Value& value) = 0;
        };
        
        class VariableTable : public VariableStore {
        private:
            typedef std::map<String, Value> Table;
            Table m_variables;
        public:
            VariableTable();
            VariableTable(const Table& variables);
        private:
            VariableStore* doClone() const;
            Value doGetValue(const String& name) const;
            StringSet doGetNames() const;
            void doDeclare(const String& name, const Value& value);
            void doAssign(const String& name, const Value& value);
        };
        
        class EvaluationContext {
        private:
            VariableStore* m_store;
        public:
            EvaluationContext();
            EvaluationContext(const VariableStore& store);
            virtual ~EvaluationContext();
            
            virtual Value variableValue(const String& name) const;
            virtual void declareVariable(const String& name, const Value& value = Value::Undefined);
        };
        
        class EvaluationStack : public EvaluationContext {
        private:
            const EvaluationContext& m_next;
        public:
            EvaluationStack(const EvaluationContext& next);
            
            Value variableValue(const String& name) const;
        };

        class ExpressionBase;
        
        class Expression {
        private:
            typedef std::tr1::shared_ptr<ExpressionBase> ExpressionPtr;
            ExpressionPtr m_expression;
        public:
            Expression(ExpressionBase* expression);

            void optimize();
            Value evaluate(const EvaluationContext& context) const;
        };
        
        class BinaryOperator;

        class ExpressionBase {
        public:
            typedef std::auto_ptr<ExpressionBase> Ptr;
            typedef std::list<ExpressionBase*> List;
            typedef std::map<String, ExpressionBase*> Map;
        protected:
            size_t m_line;
            size_t m_column;
        protected:
            static void replaceExpression(ExpressionBase*& oldExpression, ExpressionBase* newExpression);
        public:
            ExpressionBase(size_t line, size_t column);
            virtual ~ExpressionBase();

            ExpressionBase* reorderByPrecedence();
            ExpressionBase* reorderByPrecedence(BinaryOperator* parent);
            
            ExpressionBase* clone() const;
            ExpressionBase* optimize();
            Value evaluate(const EvaluationContext& context) const;
        private:
            virtual ExpressionBase* doReorderByPrecedence();
            virtual ExpressionBase* doReorderByPrecedence(BinaryOperator* parent);
            virtual ExpressionBase* doClone() const = 0;
            virtual ExpressionBase* doOptimize() = 0;
            virtual Value doEvaluate(const EvaluationContext& context) const = 0;
            
            deleteCopyAndAssignment(ExpressionBase)
        };
        
        class LiteralExpression : public ExpressionBase {
        private:
            Value m_value;
        private:
            LiteralExpression(const Value& value, size_t line, size_t column);
        public:
            static ExpressionBase* create(const Value& value, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            ExpressionBase* doOptimize();
            Value doEvaluate(const EvaluationContext& context) const;
            
            deleteCopyAndAssignment(LiteralExpression)
        };
        
        class VariableExpression : public ExpressionBase {
        private:
            String m_variableName;
        private:
            VariableExpression(const String& variableName, size_t line, size_t column);
        public:
            static ExpressionBase* create(const String& variableName, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            ExpressionBase* doOptimize();
            Value doEvaluate(const EvaluationContext& context) const;
            
            deleteCopyAndAssignment(VariableExpression)
        };
        
        class ArrayExpression : public ExpressionBase {
        private:
            ExpressionBase::List m_elements;
        private:
            ArrayExpression(const ExpressionBase::List& elements, size_t line, size_t column);
        public:
            static ExpressionBase* create(const ExpressionBase::List& elements, size_t line, size_t column);
            ~ArrayExpression();
        private:
            ExpressionBase* doClone() const;
            ExpressionBase* doOptimize();
            Value doEvaluate(const EvaluationContext& context) const;
            
            deleteCopyAndAssignment(ArrayExpression)
        };
        
        class MapExpression : public ExpressionBase {
        private:
            ExpressionBase::Map m_elements;
        private:
            MapExpression(const ExpressionBase::Map& elements, size_t line, size_t column);
        public:
            static ExpressionBase* create(const ExpressionBase::Map& elements, size_t line, size_t column);
            ~MapExpression();
        private:
            ExpressionBase* doClone() const;
            ExpressionBase* doOptimize();
            Value doEvaluate(const EvaluationContext& context) const;
            
            deleteCopyAndAssignment(MapExpression)
        };
        
        class UnaryOperator : public ExpressionBase {
        protected:
            ExpressionBase* m_operand;
        protected:
            UnaryOperator(ExpressionBase* operand, size_t line, size_t column);
        public:
            virtual ~UnaryOperator();
        private:
            ExpressionBase* doOptimize();
            deleteCopyAndAssignment(UnaryOperator)
        };

        class UnaryPlusOperator : public UnaryOperator {
        private:
            UnaryPlusOperator(ExpressionBase* operand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* operand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            
            deleteCopyAndAssignment(UnaryPlusOperator)
        };
        
        class UnaryMinusOperator : public UnaryOperator {
        private:
            UnaryMinusOperator(ExpressionBase* operand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* operand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            
            deleteCopyAndAssignment(UnaryMinusOperator)
        };
        
        class NegationOperator : public UnaryOperator {
        private:
            NegationOperator(ExpressionBase* operand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* operand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            
            deleteCopyAndAssignment(NegationOperator)
        };
        
        class GroupingOperator : public UnaryOperator {
        private:
            GroupingOperator(ExpressionBase* operand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* operand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            
            deleteCopyAndAssignment(GroupingOperator)
        };
        
        class SubscriptOperator : public ExpressionBase {
        private:
            ExpressionBase* m_indexableOperand;
            ExpressionBase* m_indexOperand;
        private:
            SubscriptOperator(ExpressionBase* indexableOperand, ExpressionBase* indexOperand, size_t line, size_t column);
        public:
            ~SubscriptOperator();
        public:
            static ExpressionBase* create(ExpressionBase* indexableOperand, ExpressionBase* indexOperand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            ExpressionBase* doOptimize();
            Value doEvaluate(const EvaluationContext& context) const;
            
            deleteCopyAndAssignment(SubscriptOperator)
        };
        
        class BinaryOperator : public ExpressionBase {
        protected:
            ExpressionBase* m_leftOperand;
            ExpressionBase* m_rightOperand;
        protected:
            BinaryOperator(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        public:
            virtual ~BinaryOperator();
        private:
            ExpressionBase* doReorderByPrecedence();
            ExpressionBase* doReorderByPrecedence(BinaryOperator* parent);
            BinaryOperator* rotateLeftUp(BinaryOperator* leftOperand);
            BinaryOperator* rotateRightUp(BinaryOperator* rightOperand);
        private:
            ExpressionBase* doOptimize();
        protected:
            struct Traits;
        private:
            Traits traits() const;
            virtual Traits doGetTraits() const = 0;
        public:
            size_t precedence() const;
            bool associative() const;
            bool commutative() const;
        private:
            
            deleteCopyAndAssignment(BinaryOperator)
        };
        
        class AdditionOperator : public BinaryOperator {
        private:
            AdditionOperator(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            Traits doGetTraits() const;
            
            deleteCopyAndAssignment(AdditionOperator)
        };
        
        class SubtractionOperator : public BinaryOperator {
        private:
            SubtractionOperator(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            Traits doGetTraits() const;
            
            deleteCopyAndAssignment(SubtractionOperator)
        };
        
        class MultiplicationOperator : public BinaryOperator {
        private:
            MultiplicationOperator(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            Traits doGetTraits() const;
            
            deleteCopyAndAssignment(MultiplicationOperator)
        };
        
        class DivisionOperator : public BinaryOperator {
        private:
            DivisionOperator(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            Traits doGetTraits() const;
            
            deleteCopyAndAssignment(DivisionOperator)
        };
        
        class ModulusOperator : public BinaryOperator {
        private:
            ModulusOperator(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            Traits doGetTraits() const;
            
            deleteCopyAndAssignment(ModulusOperator)
        };
        
        class ConjunctionOperator : public BinaryOperator {
        private:
            ConjunctionOperator(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            Traits doGetTraits() const;
            
            deleteCopyAndAssignment(ConjunctionOperator)
        };
        
        class DisjunctionOperator : public BinaryOperator {
        private:
            DisjunctionOperator(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            Traits doGetTraits() const;
            
            deleteCopyAndAssignment(DisjunctionOperator)
        };
        
        class ComparisonOperator : public BinaryOperator {
        private:
            typedef enum {
                Op_Less,
                Op_LessOrEqual,
                Op_Equal,
                Op_Inequal,
                Op_GreaterOrEqual,
                Op_Greater
            } Op;
            Op m_op;
        private:
            ComparisonOperator(ExpressionBase* leftOperand, ExpressionBase* rightOperand, Op op, size_t line, size_t column);
        public:
            static ExpressionBase* createLess(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
            static ExpressionBase* createLessOrEqual(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
            static ExpressionBase* createEqual(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
            static ExpressionBase* createInequal(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
            static ExpressionBase* createGreaterOrEqual(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
            static ExpressionBase* createGreater(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            Traits doGetTraits() const;
            
            deleteCopyAndAssignment(ComparisonOperator)
        };
        
        class RangeOperator : public BinaryOperator {
        public:
            static const String& AutoRangeParameterName();
        private:
            RangeOperator(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
        public:
            static ExpressionBase* create(ExpressionBase* leftOperand, ExpressionBase* rightOperand, size_t line, size_t column);
            static ExpressionBase* createAutoRangeWithLeftOperand(ExpressionBase* leftOperand, size_t line, size_t column);
            static ExpressionBase* createAutoRangeWithRightOperand(ExpressionBase* rightOperand, size_t line, size_t column);
        private:
            ExpressionBase* doClone() const;
            Value doEvaluate(const EvaluationContext& context) const;
            Traits doGetTraits() const;
            
            deleteCopyAndAssignment(RangeOperator)
        };
    }
}

#endif /* EL_h */
