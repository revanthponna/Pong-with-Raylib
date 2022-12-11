#include "raylib.h"

// Making structure for ball
struct Ball 
{
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw() {
		DrawCircle((int) x, (int) y, radius, WHITE);
	}
};

// Making structure for paddles
struct Paddle
{
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect() {
		return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
	}

	void Draw() {
		DrawRectangleRec(GetRect(), WHITE);
	}
};

int main() 
{
	InitWindow(800, 600, "Pong"); // Setting dimensions of window
	SetWindowState(FLAG_VSYNC_HINT);

	// Creating ball object at center of screen
	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius= 5;
	ball.speedX= 300;
	ball.speedY= 300;

	// Creating left paddle object
	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;

	// Creating right paddle object
	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;

	const char* winnerText = nullptr;

	while (!WindowShouldClose()) {

		// Making the ball move with the specified speed
		ball.x += ball.speedX * GetFrameTime(); 
		ball.y += ball.speedY * GetFrameTime();

		// Making the ball bounce off the top and bottom of screen
		if (ball.y < 0) {
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight()) {
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}

		// Input controls for the left and right paddles
		if (IsKeyDown(KEY_W)) {
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S)) {
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP)) {
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN)) {
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		// Checking for collision between ball and paddles
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect())) {
			if (ball.speedX < 0) {
				ball.speedX *= -1.1f; // Increasing the ball speed with every collision as the game progresses
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX; // Making the ball bounce off depending on where it hits on the paddle
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect())) {
			if (ball.speedX > 0) {
				ball.speedX *= -1.1f; // Increasing the ball speed with every collision as the game progresses
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX; // Making the ball bounce off depending on where it hits on the paddle
			}
		}

		// Deciding the winner of the game
		if (ball.x < 0) {
			winnerText = "Right Player Wins!";
		}
		if (ball.x > GetScreenWidth()) {
			winnerText = "Left Player Wins!";
		}

		// Restarting the game if Space key is pressed
		if (winnerText && IsKeyPressed(KEY_SPACE)) {
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 300;
			ball.speedY = 300;
			winnerText = nullptr;
		}
		
		BeginDrawing();
			ClearBackground(BLACK);

			// Displaying our objects in the window
			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();

			// Displaying the winner
			if (winnerText) {
				int textWidth = MeasureText(winnerText, 60);
				DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
			}

			DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}