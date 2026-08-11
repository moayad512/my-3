import turtle

# 1. إعداد شاشة اللعبة
screen = turtle.Screen()
screen.title("Python Pong Game")
screen.bgcolor("black")
screen.setup(width=800, height=600)
screen.tracer(0)  # إيقاف التحديث التلقائي

# 2. المضرب الأيسر
paddle_a = turtle.Turtle()
paddle_a.speed(0)
paddle_a.shape("square")
paddle_a.color("white")
paddle_a.shapesize(stretch_wid=6, stretch_len=1)
paddle_a.penup()
paddle_a.goto(-350, 0)

# 3. المضرب الأيمن
paddle_b = turtle.Turtle()
paddle_b.speed(0)
paddle_b.shape("square")
paddle_b.color("white")
paddle_b.shapesize(stretch_wid=6, stretch_len=1)
paddle_b.penup()
paddle_b.goto(350, 0)

# 4. الكرة
ball = turtle.Turtle()
ball.speed(0)
ball.shape("circle")
ball.color("white")
ball.penup()
ball.goto(0, 0)
ball.dx = 2  # سرعة الحركة الأفقية
ball.dy = 2  # سرعة الحركة الرأسية

# 5. لوحة النقاط
scoreboard = turtle.Turtle()
scoreboard.speed(0)
scoreboard.color("white")
scoreboard.penup()
scoreboard.hideturtle()
scoreboard.goto(0, 260)

score_a = 0
score_b = 0
scoreboard.write("Player A: 0  Player B: 0", align="center", font=("Courier", 24, "normal"))

# 6. دوال حركة المضارب
def paddle_a_up():
    y = paddle_a.ycor()
    if y < 250:
        y += 20
    paddle_a.sety(y)

def paddle_a_down():
    y = paddle_a.ycor()
    if y > -240:
        y -= 20
    paddle_a.sety(y)

def paddle_b_up():
    y = paddle_b.ycor()
    if y < 250:
        y += 20
    paddle_b.sety(y)

def paddle_b_down():
    y = paddle_b.ycor()
    if y > -240:
        y -= 20
    paddle_b.sety(y)

# 7. ربط المفاتيح
screen.listen()
screen.onkeypress(paddle_a_up, "w")
screen.onkeypress(paddle_a_down, "s")
screen.onkeypress(paddle_b_up, "Up")
screen.onkeypress(paddle_b_down, "Down")

# 8. الحلقة الرئيسية للعبة
while True:
    screen.update()

    # تحريك الكرة
    ball.setx(ball.xcor() + ball.dx)
    ball.sety(ball.ycor() + ball.dy)

    # الارتداد من الحافة العلوية
    if ball.ycor() > 290:
        ball.sety(290)
        ball.dy *= -1

    # الارتداد من الحافة السفلية
    if ball.ycor() < -290:
        ball.sety(-290)
        ball.dy *= -1

    # تسجيل نقطة للاعب B (الكرة تجاوزت اللاعب A)
    if ball.xcor() > 390:
        ball.goto(0, 0)
        ball.dx *= -1
        score_a += 1
        scoreboard.clear()
        scoreboard.write(f"Player A: {score_a}  Player B: {score_b}", align="center", font=("Courier", 24, "normal"))

    # تسجيل نقطة للاعب A (الكرة تجاوزت اللاعب B)
    if ball.xcor() < -390:
        ball.goto(0, 0)
        ball.dx *= -1
        score_b += 1
        scoreboard.clear()
        scoreboard.write(f"Player A: {score_a}  Player B: {score_b}", align="center", font=("Courier", 24, "normal"))

    # اصطدام الكرة بالمضرب الأيمن
    if (ball.xcor() > 340 and ball.xcor() < 350) and (ball.ycor() < paddle_b.ycor() + 50 and ball.ycor() > paddle_b.ycor() - 50):
        ball.setx(340)
        ball.dx *= -1

    # اصطدام الكرة بالمضرب الأيسر
    if (ball.xcor() < -340 and ball.xcor() > -350) and (ball.ycor() < paddle_a.ycor() + 50 and ball.ycor() > paddle_a.ycor() - 50):
        ball.setx(-340)
        ball.dx *= -1