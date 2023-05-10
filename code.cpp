# c-code
using System;
using System.Drawing;
using System.Windows.Forms;

namespace Minesweeper
{
    public partial class Form1 : Form
    {
        private const int Rows = 10;
        private const int Cols = 10;
        private const int Mines = 15;
        private readonly Button[,] _buttons = new Button[Rows, Cols];
        private readonly bool[,] _mines = new bool[Rows, Cols];
        private readonly int[,] _counts = new int[Rows, Cols];
        private int _revealed;

        public Form1()
        {
            InitializeComponent();
            InitBoard();
            PlaceMines();
            CalculateCounts();
        }

        private void InitBoard()
        {
            const int buttonSize = 30;
            const int margin = 5;

            for (var row = 0; row < Rows; row++)
            {
                for (var col = 0; col < Cols; col++)
                {
                    var button = new Button
                    {
                        Size = new Size(buttonSize, buttonSize),
                        Location = new Point(col * (buttonSize + margin), row * (buttonSize + margin)),
                        Tag = new Tuple<int, int>(row, col)
                    };
                    button.MouseDown += ButtonOnMouseDown;
                    Controls.Add(button);
                    _buttons[row, col] = button;
                }
            }

            var formWidth = Cols * buttonSize + (Cols - 1) * margin;
            var formHeight = Rows * buttonSize + (Rows - 1) * margin + menuStrip1.Height;
            ClientSize = new Size(formWidth, formHeight);
        }

        private void PlaceMines()
        {
            var rnd = new Random();
            for (var i = 0; i < Mines; i++)
            {
                int row, col;
                do
                {
                    row = rnd.Next(Rows);
                    col = rnd.Next(Cols);
                } while (_mines[row, col]);
                _mines[row, col] = true;
            }
        }

        private void CalculateCounts()
        {
            for (var row = 0; row < Rows; row++)
            {
                for (var col = 0; col < Cols; col++)
                {
                    if (_mines[row, col])
                    {
                        _counts[row, col] = -1;
                    }
                    else
                    {
                        var count = 0;
                        for (var r = row - 1; r <= row + 1; r++)
                        {
                            for (var c = col - 1; c <= col + 1; c++)
                            {
                                if (r >= 0 && r < Rows && c >= 0 && c < Cols && _mines[r, c])
                                {
                                    count++;
                                }
                            }
                        }
                        _counts[row, col] = count;
                    }
                }
            }
        }

        private void ButtonOnMouseDown(object sender, MouseEventArgs e)
        {
            var button = (Button) sender;
            var (row, col) = (Tuple<int, int>) button.Tag;

            if (e.Button == MouseButtons.Left)
            {
                if (_mines[row, col])
                {
                    MessageBox.Show("Game over!");
                    ResetBoard();
                }
                else if (_counts[row, col] > 0)
                {
                    button.Text = _counts[row, col].ToString();
                    button.Enabled = false;
                    _revealed++;
                    if (_revealed == Rows * Cols - Mines)
                    {
                        MessageBox.Show("You win
