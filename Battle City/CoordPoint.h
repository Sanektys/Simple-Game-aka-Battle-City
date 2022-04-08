#pragma once


/// <summary>
/// Перечисление типов размещения CoordPoint внутри целевого объекта
/// <para>(верхняя левая четверть, верхняя правая четверть,
/// нижняя левая четверть и нижняя правая четверть)</para>
/// </summary>
enum class TypeCoordPoint
{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT
};

/// <summary>
/// Структура, являющая собою подобъект игрового объекта,
/// размерами равную 1/4 от целевого объекта
/// <para>Предназначена для более подробной
/// обработки коллизии между объектами</para>
/// </summary>
struct CoordPoint
{
    CoordPoint() = default;
    CoordPoint(class GameObject* modifyObject,
               enum TypeCoordPoint typePoint) {
        setCoordPoint(modifyObject, typePoint);
    }
    CoordPoint(float coordX, float coordY, float width, float height)
        : coordX(coordX), coordY(coordY), width(width), height(height) {}

    /// <summary>
    /// Метод, устанавливающий значения объекта CoordPoint согласно
    /// целевому объекту GameObject
    /// </summary>
    /// <param name="modifyObject">- целевой объект, на основе которого
    /// создаётся объект CoordPoint равный 1/4 размеров первого
    /// и внутри его границ</param>
    /// <param name="typePoint">- вариация расположения объекта CoordPoint
    /// внутри целевого объекта</param>
    void setCoordPoint(class GameObject* modifyObject,
                       enum TypeCoordPoint typePoint);

    // Начальная горизонтальная координата четвертинки искомого объекта
    float coordX{0.0f};
    // Начальная вертикальная координата четвертинки искомого объекта
    float coordY{0.0f};
    // Ширина четвертинки искомого объекта
    float width {0.1f};
    // Высота четвертинки искомого объекта
    float height{0.1f};
};