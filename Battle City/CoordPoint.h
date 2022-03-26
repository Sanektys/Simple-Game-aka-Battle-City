#pragma once


/// <summary>
/// ������������ ����� ���������� CoordPoint ������ �������� �������
/// <para>(������� ����� ��������, ������� ������ ��������,
/// ������ ����� �������� � ������ ������ ��������)</para>
/// </summary>
enum class typeCoordPoint
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
		               enum class typeCoordPoint typePoint);

    // ��������� �������������� ���������� ����������� �������� �������
    float coordX{0.0f};
    // ��������� ������������ ���������� ����������� �������� �������
    float coordY{0.0f};
    // ������ ����������� �������� �������
    float width {0.1f};
    // ������ ����������� �������� �������
    float height{0.1f};
};