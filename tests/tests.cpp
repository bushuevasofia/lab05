#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Account.h"
#include "Transaction.h"

using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

// Mock-класс для Account 
class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}

    MOCK_CONST_METHOD0(GetBalance, int());
    MOCK_METHOD1(ChangeBalance, void(int diff));
    MOCK_METHOD0(Lock, void());
    MOCK_METHOD0(Unlock, void());
};

// Тесты для Account 

TEST(AccountTest, LockUnlockBehaviour) {
    MockAccount acc(0, 1000);

    EXPECT_CALL(acc, Lock()).Times(2);
    EXPECT_CALL(acc, Unlock()).Times(1);

    acc.Lock();
    acc.Lock();   
    acc.Unlock();
}

TEST(AccountTest, BalanceModificationWithLock) {
    Account acc(0, 1000);

    EXPECT_EQ(acc.GetBalance(), 1000);

    acc.Lock();
    EXPECT_NO_THROW(acc.ChangeBalance(100)); 

    EXPECT_EQ(acc.GetBalance(), 1100);
}

TEST(AccountTest, ChangeBalanceWithoutLockThrows) {
    Account acc(0, 1000);

    EXPECT_THROW(acc.ChangeBalance(100), std::runtime_error);  

    acc.Lock();
    EXPECT_NO_THROW(acc.ChangeBalance(100));  

    EXPECT_EQ(acc.GetBalance(), 1100);
}

TEST(AccountTest, RepeatedLockThrows) {
    Account acc(0, 1000);

    acc.Lock();
    EXPECT_THROW(acc.Lock(), std::runtime_error);  
}

// Тесты для Transaction 

TEST(TransactionTest, ConstructorAndFee) {
    Transaction tx;
    EXPECT_EQ(tx.fee(), 1);

    tx.set_fee(32);
    EXPECT_EQ(tx.fee(), 32);
}

// Успешный перевод: проверяются балансы до и после
TEST(TransactionTest, SuccessfulTransfer) {
    Account from(0, 6132);
    Account to(1, 2133);

    Transaction tx;
    tx.set_fee(32);

    bool result = tx.Make(from, to, 100);
    EXPECT_TRUE(result);

    // ИСПРАВЛЕНО: комиссия списывается с отправителя
    EXPECT_EQ(from.GetBalance(), 6132 - (100 + 32));  
    EXPECT_EQ(to.GetBalance(), 2133 + 100);         
}

// Тест на некорректные входные данные
TEST(TransactionTest, InvalidTransfers) {
    Transaction tx;
    tx.set_fee(51);

    Account same(0, 1000);
    Account poor(1, 10);
    Account rich(2, 1000);

    // Перевод самому себе
    EXPECT_THROW(tx.Make(same, same, 0), std::logic_error);

    // Отрицательная сумма
    EXPECT_THROW(tx.Make(rich, poor, -100), std::invalid_argument);

    // Слишком маленькая сумма
    EXPECT_THROW(tx.Make(rich, poor, 50), std::logic_error);

    // Комиссия больше суммы
    EXPECT_FALSE(tx.Make(rich, poor, 100));

    // Недостаточно средств
    tx.set_fee(10);
    EXPECT_FALSE(tx.Make(poor, rich, 100));  
}
