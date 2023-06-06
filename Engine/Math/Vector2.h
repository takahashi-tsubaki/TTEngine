#pragma once

/// <summary>
/// 2�����x�N�g��
/// </summary>
class Vector2 {
public:
	float x; // x����
	float y; // y����

public:
	// �R���X�g���N�^
	Vector2();                          // ��x�N�g���Ƃ���
	Vector2(float x, float y); // x����, y���� ���w�肵�Ă̐���

	// �P�����Z�q�I�[�o�[���[�h
	Vector2 operator+() const;
	Vector2 operator-() const;

	// ������Z�q�I�[�o�[���[�h
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);
};
