#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include <typeinfo>

#include "factory.h"
#include "druid.h"
#include "orc.h"
#include "slaver.h"

// Тестирование создания NPC из потока
TEST(FactoryTest, CreateFromStream) {
    // Test Slaver
    {
        std::stringstream ss("0 100 200"); // Type=1 (Slaver), x=100, y=200
        auto npc = factory(ss);
        ASSERT_NE(npc, nullptr);
        EXPECT_EQ(npc->get_type(), SlaverType);
        EXPECT_EQ(typeid(*npc), typeid(Slaver));
        EXPECT_EQ(npc->get_x(), 100);
        EXPECT_EQ(npc->get_y(), 200);
    }

    // Test Orc
    {
        std::stringstream ss("1 150 250"); // Type=2 (Orc), x=150, y=250
        auto npc = factory(ss);
        ASSERT_NE(npc, nullptr);
        EXPECT_EQ(npc->get_type(), OrcType);
        EXPECT_EQ(typeid(*npc), typeid(Orc));
        EXPECT_EQ(npc->get_x(), 150);
        EXPECT_EQ(npc->get_y(), 250);
    }

    // Test Druid
    {
        std::stringstream ss("2 300 400"); // Type=3 (Druid), x=300, y=400
        auto npc = factory(ss);
        ASSERT_NE(npc, nullptr);
        EXPECT_EQ(npc->get_type(), DruidType);
        EXPECT_EQ(typeid(*npc), typeid(Druid));
        EXPECT_EQ(npc->get_x(), 300);
        EXPECT_EQ(npc->get_y(), 400);
    }
}

// Тестирование некорректного ввода из потока
TEST(FactoryTest, InvalidStreamInput) {
    // Пустой поток
    {
        std::stringstream ss("");
        auto npc = factory(ss);
        EXPECT_EQ(npc, nullptr);
    }

    // Неверный тип
    {
        std::stringstream ss("99 100 200"); // Type=99 не существует
        auto npc = factory(ss);
        EXPECT_EQ(npc, nullptr);
    }

    // Неполные данные
    {
        std::stringstream ss("0 100"); // Только тип и x, нет y
        EXPECT_THROW(auto npc = factory(ss), std::invalid_argument);
    }

    // Текст вместо чисел
    {
        std::stringstream ss("abc def ghi");
        auto npc = factory(ss);
        EXPECT_EQ(npc, nullptr);
    }
}

// Тестирование создания NPC по типу и координатам
TEST(FactoryTest, CreateByTypeAndCoords) {
    // Test Slaver
    {
        auto npc = factory(SlaverType, 100, 200);
        ASSERT_NE(npc, nullptr);
        EXPECT_EQ(npc->get_type(), SlaverType);
        EXPECT_EQ(typeid(*npc), typeid(Slaver));
        EXPECT_EQ(npc->get_x(), 100);
        EXPECT_EQ(npc->get_y(), 200);
    }

    // Test Orc
    {
        auto npc = factory(OrcType, 150, 250);
        ASSERT_NE(npc, nullptr);
        EXPECT_EQ(npc->get_type(), OrcType);
        EXPECT_EQ(typeid(*npc), typeid(Orc));
        EXPECT_EQ(npc->get_x(), 150);
        EXPECT_EQ(npc->get_y(), 250);
    }

    // Test Druid
    {
        auto npc = factory(DruidType, 300, 400);
        ASSERT_NE(npc, nullptr);
        EXPECT_EQ(npc->get_type(), DruidType);
        EXPECT_EQ(typeid(*npc), typeid(Druid));
        EXPECT_EQ(npc->get_x(), 300);
        EXPECT_EQ(npc->get_y(), 400);
    }
}

// Тестирование сериализации/десериализации
TEST(FactoryTest, SerializationDeserialization) {
    // Создаем NPC, сохраняем в поток, загружаем обратно
    auto original = factory(DruidType, 123, 456);
    ASSERT_NE(original, nullptr);

    std::stringstream ss;
    original->save(ss);

    // Перемотка потока
    ss.seekg(0);

    auto loaded = factory(ss);
    ASSERT_NE(loaded, nullptr);

    // Проверяем, что загруженный объект идентичен оригиналу
    EXPECT_EQ(loaded->get_type(), original->get_type());
    EXPECT_EQ(loaded->get_x(), original->get_x());
    EXPECT_EQ(loaded->get_y(), original->get_y());
    EXPECT_EQ(typeid(*loaded), typeid(*original));
}

// Тестирование поведения NPC после создания
TEST(FactoryTest, NPCMethodsAfterCreation) {
    auto slaver = factory(SlaverType, 100, 200);
    auto orc = factory(OrcType, 105, 205);
    auto druid = factory(DruidType, 300, 400);

    ASSERT_NE(slaver, nullptr);
    ASSERT_NE(orc, nullptr);
    ASSERT_NE(druid, nullptr);

    // Проверяем, что NPC могут представляться
    testing::internal::CaptureStdout();
    slaver->print(std::cout);
    std::string slaverOutput = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(slaverOutput.empty());

    testing::internal::CaptureStdout();
    orc->print(std::cout);
    std::string orcOutput = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(orcOutput.empty());

    testing::internal::CaptureStdout();
    druid->print(std::cout);
    std::string druidOutput = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(druidOutput.empty());

    // Проверяем метод is_close
    EXPECT_TRUE(slaver->is_close(*orc)); // Расстояние ~71 < 100
    EXPECT_FALSE(slaver->is_close(*druid)); // Расстояние ~361 > 100
}

// Тестирование наблюдателей
TEST(FactoryTest, ObserverAttached) {
    auto npc = factory(SlaverType, 100, 200);
    ASSERT_NE(npc, nullptr);

    // Проверяем, что наблюдатели подключены
    // (зависит от реализации, может потребоваться метод для проверки)

    // Если в NPC есть метод для получения наблюдателей:
    // EXPECT_FALSE(npc->get_observers().empty());
}

// Тестирование всех возможных типов
TEST(FactoryTest, AllNpcTypes) {
    // Проходим по всем известным типам NPC
    for (int typeInt = static_cast<int>(SlaverType);
         typeInt <= static_cast<int>(DruidType);
         ++typeInt) {
        NpcType type = static_cast<NpcType>(typeInt);

        auto npc = factory(type, typeInt * 10, typeInt * 20);
        ASSERT_NE(npc, nullptr) << "Failed to create NPC type: " << typeInt;

        EXPECT_EQ(npc->get_type(), type);
        EXPECT_EQ(npc->get_x(), typeInt * 10);
        EXPECT_EQ(npc->get_y(), typeInt * 20);
    }
}

// Тестирование граничных координат
TEST(FactoryTest, BoundaryCoordinates) {
    // Минимальные координаты
    {
        auto npc = factory(OrcType, 0, 0);
        ASSERT_NE(npc, nullptr);
        EXPECT_EQ(npc->get_x(), 0);
        EXPECT_EQ(npc->get_y(), 0);
    }

    // Максимальные координаты (в пределах int)
    {
        auto npc = factory(SlaverType, 1000, 1000);
        ASSERT_NE(npc, nullptr);
        EXPECT_EQ(npc->get_x(), 1000);
        EXPECT_EQ(npc->get_y(), 1000);
    }

    // Отрицательные координаты
    {
        auto npc = factory(DruidType, -100, -200);
        ASSERT_NE(npc, nullptr);
        EXPECT_EQ(npc->get_x(), -100);
        EXPECT_EQ(npc->get_y(), -200);
    }
}

// Параметризованный тест для разных типов
class FactoryTypeTest : public testing::TestWithParam<NpcType> {
protected:
    void SetUp() override {
        npcType = GetParam();
    }

    NpcType npcType;
};

TEST_P(FactoryTypeTest, CreateAndVerify) {
    auto npc = factory(npcType, 100, 200);
    ASSERT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_type(), npcType);
    EXPECT_EQ(npc->get_x(), 100);
    EXPECT_EQ(npc->get_y(), 200);

    // Проверяем, что создан правильный тип
    switch (npcType) {
        case SlaverType:
            EXPECT_EQ(typeid(*npc), typeid(Slaver));
            break;
        case OrcType:
            EXPECT_EQ(typeid(*npc), typeid(Orc));
            break;
        case DruidType:
            EXPECT_EQ(typeid(*npc), typeid(Druid));
            break;
        default:
            FAIL() << "Unknown NPC type";
    }
}

// Регистрируем параметры для теста
INSTANTIATE_TEST_SUITE_P(
    AllNpcTypes,
    FactoryTypeTest,
    testing::Values(
        SlaverType,
        OrcType,
        DruidType
    )
);

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
