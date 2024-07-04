#pragma once

namespace Render
{
	void Line(FVector2D a, FVector2D b, FLinearColor color, float thickness)
	{
		Variables::Canvas->K2_DrawLine(a, b, thickness, color);
	}

	void Text(FString content, FVector2D screen, FLinearColor color, bool center_x, bool center_y, bool outlined, double scale = 1.00)
	{
		Variables::Canvas->K2_DrawText(content, screen, FVector2D(scale, scale), color, scale, center_x, center_y, outlined);
	}

	void DrawRectFilled(FVector2D position, FVector2D size, FLinearColor render_color)
	{
		for (int i = 0; i < size.Y; i++)
		{
			Render::Line(FVector2D(position.X, position.Y + i), FVector2D(position.X + size.X, position.Y + i), render_color, 1.f);
		}
	}

	void RoundedRectFilled(FVector2D PositionA, FVector2D PositionB, FLinearColor RenderColor, double Radius, double Segments)
	{
		double Step = M_PI * 2.0 / Segments;

		for (double deg = 0; deg < M_PI * 2; deg += Step)
		{
			double x1 = Radius * UKismetMathLibrary::cos(deg) + PositionA.X;
			double y1 = Radius * UKismetMathLibrary::sin(deg) + PositionA.Y;
			double x2 = Radius * UKismetMathLibrary::cos(deg + Step) + PositionA.X;
			double y2 = Radius * UKismetMathLibrary::sin(deg + Step) + PositionA.Y;

			Render::Line(FVector2D(x1, y1), FVector2D(x2, y2), RenderColor, 1.f);
		}
	}

	void DrawRect(FVector2D position, FVector2D size, FLinearColor render_color, float thickness)
	{
		Render::Line(FVector2D(position.X, position.Y), FVector2D(position.X + size.X, position.Y), render_color, thickness);
		Render::Line(FVector2D(position.X + size.X, position.Y), FVector2D(position.X + size.X, position.Y + size.Y), render_color, thickness);
		Render::Line(FVector2D(position.X + size.X, position.Y + size.Y), FVector2D(position.X, position.Y + size.Y), render_color, thickness);
		Render::Line(FVector2D(position.X, position.Y + size.Y), FVector2D(position.X, position.Y), render_color, thickness);
	}

	void Circle(FVector2D position, FLinearColor color, double radius, double segments, bool filled)
	{
		double step = M_PI * 2.0 / segments;
		int count = 0;
		FVector2D vectors[128];

		for (double deg = 0; deg < M_PI * 2; deg += step)
		{
			double x_1 = radius * UKismetMathLibrary::cos(deg) + position.X;
			double y_1 = radius * UKismetMathLibrary::sin(deg) + position.Y;
			double x_2 = radius * UKismetMathLibrary::cos(deg + step) + position.X;
			double y_2 = radius * UKismetMathLibrary::sin(deg + step) + position.Y;

			vectors[count].X = x_1;
			vectors[count].Y = y_1;
			vectors[count + 1].X = x_2;
			vectors[count + 1].Y = y_2;

			if (filled)
			{
				for (double X = x_1; X <= x_2; X += 1.0)
				{
					double Y = position.Y + radius * UKismetMathLibrary::sin(UKismetMathLibrary::acos((X - position.X) / radius));
					Render::Line(FVector2D(X, y_1), FVector2D(X, Y), color, 1.0f);
					Render::Line(FVector2D(X, Y), FVector2D(X, y_2), color, 1.0f);
				}
			}
			else
			{
				Render::Line(FVector2D(vectors[count].X, vectors[count].Y), FVector2D(x_2, y_2), color, 1.0f);
			}
		}
	}
}

namespace Wrapper
{
	struct DrawListMember
	{
		int DrawType;

		FString Content;

		FVector2D PositionA, PositionB;
		FLinearColor Color;

		float Thickness;

		double Radius;
		double Segments;
		bool Filled;

		bool Outlined;
		bool Centered;

		bool Rounded;
	};

	int LastForegroundIndex = 0;
	DrawListMember ForegroundDrawList[9999];

	int LastDefaultIndex = 0;
	DrawListMember DefaultDrawList[9999];

	void ClearTargets()
	{
		LastDefaultIndex = 0;
		LastForegroundIndex = 0;
	}

	void Render()
	{
		for (int i = 0; i < LastDefaultIndex; i++)
		{
			DrawListMember Member = DefaultDrawList[i];

			if (Member.DrawType == 1)
			{
				Render::Line(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 2)
			{
				Render::DrawRect(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 3)
			{
				Render::DrawRectFilled(Member.PositionA, Member.PositionB, Member.Color);
			}
			else if (Member.DrawType == 4)
			{
				Render::Text(Member.Content, Member.PositionA, Member.Color, Member.Centered, false, Member.Outlined);
			}
			else if (Member.DrawType == 5)
			{
				Render::Circle(Member.PositionA, Member.Color, Member.Radius, Member.Segments, Member.Filled);
			}
		}

		for (int i = 0; i < LastForegroundIndex; i++)
		{
			DrawListMember Member = ForegroundDrawList[i];

			if (Member.DrawType == 1)
			{
				Render::Line(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 2)
			{
				Render::DrawRect(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 3)
			{
				Render::DrawRectFilled(Member.PositionA, Member.PositionB, Member.Color);
			}
			else if (Member.DrawType == 4)
			{
				Render::Text(Member.Content, Member.PositionA, Member.Color, Member.Centered, false, Member.Outlined);
			}
			else if (Member.DrawType == 5)
			{
				Render::Circle(Member.PositionA, Member.Color, Member.Radius, Member.Segments, Member.Filled);
			}
		}
	}

	void Line(FVector2D PositionA, FVector2D PositionB, FLinearColor RenderColor, float Thickness, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 1;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Color = RenderColor;
		Member.Thickness = Thickness;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void Rect(FVector2D PositionA, FVector2D PositionB, FLinearColor RenderColor, float Thickness, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 2;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Color = RenderColor;
		Member.Thickness = Thickness;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void RectFilled(FVector2D PositionA, FVector2D PositionB, FLinearColor RenderColor, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 3;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Color = RenderColor;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void Text(FString Content, FVector2D Position, FLinearColor RenderColor, bool Centered = false, bool Outlined = false, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 4;
		Member.Content = FString(Content.c_str(), Content.Size());
		Member.PositionA = Position;
		Member.Color = RenderColor;
		Member.Centered = Centered;
		Member.Outlined = Outlined;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void Circle(FVector2D Position, FLinearColor RenderColor, double Radius, double Segments, bool Filled = false, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 5;
		Member.PositionA = Position;
		Member.Color = RenderColor;
		Member.Radius = Radius;
		Member.Segments = Segments;
		Member.Filled = Filled;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}
}

namespace Radar
{
	void Range(double* X, double* Y, float range)
	{
		if (UKismetMathLibrary::abs((*X)) > range || UKismetMathLibrary::abs((*Y)) > range)
		{
			if ((*Y) > (*X))
			{
				if ((*Y) > -(*X))
				{
					(*X) = range * (*X) / (*Y);
					(*Y) = range;
				}
				else
				{
					(*Y) = -range * (*Y) / (*X);
					(*X) = -range;
				}
			}
			else
			{
				if ((*Y) > -(*X))
				{
					(*Y) = range * (*Y) / (*X);
					(*X) = range;
				}
				else
				{
					(*X) = -range * (*X) / (*Y);
					(*Y) = -range;
				}
			}
		}
	}

	FVector2D RotatePoint(FVector2D radar_pos, FVector2D radar_size, FVector LocalLocation, FVector TargetLocation)
	{
		auto dx = TargetLocation.X - LocalLocation.X;
		auto dy = TargetLocation.Y - LocalLocation.Y;

		auto X = dy * -1;
		X *= -1;
		auto Y = dx * -1;

		double calcualted_range = 34 * 1000;

		Radar::Range(&X, &Y, calcualted_range);

		int rad_x = (int)radar_pos.X;
		int rad_y = (int)radar_pos.Y;

		double r_siz_x = radar_size.X;
		double r_siz_y = radar_size.Y;

		int x_max = (int)r_siz_x + rad_x - 5;
		int y_max = (int)r_siz_y + rad_y - 5;

		auto return_value = FVector2D();

		return_value.X = rad_x + ((int)r_siz_x / 2 + int(X / calcualted_range * r_siz_x));
		return_value.Y = rad_y + ((int)r_siz_y / 2 + int(Y / calcualted_range * r_siz_y));

		if (return_value.X > x_max)
			return_value.X = x_max;

		if (return_value.X < rad_x)
			return_value.X = rad_x;

		if (return_value.Y > y_max)
			return_value.Y = y_max;

		if (return_value.Y < rad_y)
			return_value.Y = rad_y;

		return return_value;
	}

	void Add(FVector WorldLocation, FVector LocalLocation, FLinearColor Color, FVector2D RadarPos, FVector2D RadarSize, double Size)
	{
		FVector2D Projected = Radar::RotatePoint(RadarPos, RadarSize, LocalLocation, WorldLocation);

		Wrapper::Circle(Projected, Color, Size, 38, false, true);
	}
}

namespace Custom
{
	FMatrix RotatorToMatrix(FRotator Rotation)
	{
		FMatrix Copy = {};

		double Pitch = (Rotation.Pitch * M_PI / 180.0);
		double Yaw = (Rotation.Yaw * M_PI / 180.0);
		double Roll = (Rotation.Roll * M_PI / 180.0);

		double SP = UKismetMathLibrary::sin(Pitch);
		double CP = UKismetMathLibrary::cos(Pitch);
		double SY = UKismetMathLibrary::sin(Yaw);
		double CY = UKismetMathLibrary::cos(Yaw);
		double SR = UKismetMathLibrary::sin(Roll);
		double CR = UKismetMathLibrary::cos(Roll);

		Copy.XPlane.X = CP * CY;
		Copy.XPlane.Y = CP * SY;
		Copy.XPlane.Z = SP;

		Copy.YPlane.X = SR * SP * CY - CR * SY;
		Copy.YPlane.Y = SR * SP * SY + CR * CY;
		Copy.YPlane.Z = -SR * CP;

		Copy.ZPlane.X = -(CR * SP * CY + SR * SY);
		Copy.ZPlane.Y = CY * SR - CR * SP * SY;
		Copy.ZPlane.Z = CR * CP;

		Copy.WPlane.W = 1.0;

		return Copy;
	}

	void WorldToScreenRaw(FVector WorldLocation, FVector2D* ScreenLocation)
	{
		FMatrix Matrix = RotatorToMatrix(Variables::CameraRotation);

		FVector XAxis = FVector(Matrix.XPlane.X, Matrix.XPlane.Y, Matrix.XPlane.Z);
		FVector YAxis = FVector(Matrix.YPlane.X, Matrix.YPlane.Y, Matrix.YPlane.Z);
		FVector ZAxis = FVector(Matrix.ZPlane.X, Matrix.ZPlane.Y, Matrix.ZPlane.Z);

		FVector DeltaCoordinates = WorldLocation - Variables::CameraLocation;

		FVector Transform = FVector(DeltaCoordinates.Dot(YAxis), DeltaCoordinates.Dot(ZAxis), DeltaCoordinates.Dot(XAxis));
		if (Transform.Z < 0.01)
		{
			Transform.Z = 0.01;
		}

		ScreenLocation->X = Variables::ScreenCenter.X + Transform.X * (Variables::ScreenCenter.X / UKismetMathLibrary::tan(Variables::FieldOfView * M_PI / 360.0)) / Transform.Z;
		ScreenLocation->Y = Variables::ScreenCenter.Y - Transform.Y * (Variables::ScreenCenter.X / UKismetMathLibrary::tan(Variables::FieldOfView * M_PI / 360.0)) / Transform.Z;
	}
}