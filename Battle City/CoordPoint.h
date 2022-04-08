#pragma once


/// <summary>
/// ������������ ����� ���������� CoordPoint ������ �������� �������
/// <para>(������� ����� ��������, ������� ������ ��������,
/// ������ ����� �������� � ������ ������ ��������)</para>
/// </summary>
enum class TypeCoordPoint
{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT
};

/// <summary>
/// ���������, �������� ����� ��������� �������� �������,
/// ��������� ������ 1/4 �� �������� �������
/// <para>������������� ��� ����� ���������
/// ��������� �������� ����� ���������</para>
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
    /// �����, ��������������� �������� ������� CoordPoint ��������
    /// �������� ������� GameObject
    /// </summary>
    /// <param name="modifyObject">- ������� ������, �� ������ ��������
    /// �������� ������ CoordPoint ������ 1/4 �������� �������
    /// � ������ ��� ������</param>
    /// <param name="typePoint">- �������� ������������ ������� CoordPoint
    /// ������ �������� �������</param>
    void setCoordPoint(class GameObject* modifyObject,
                       enum TypeCoordPoint typePoint);

    // ��������� �������������� ���������� ����������� �������� �������
    float coordX{0.0f};
    // ��������� ������������ ���������� ����������� �������� �������
    float coordY{0.0f};
    // ������ ����������� �������� �������
    float width {0.1f};
    // ������ ����������� �������� �������
    float height{0.1f};
};