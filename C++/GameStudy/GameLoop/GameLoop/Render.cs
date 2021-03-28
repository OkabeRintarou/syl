using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Configuration;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Tao.OpenGl;

namespace GameLoop
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector
    {
        public double X { get; set; }
        public double Y { get; set; }
        public double Z { get; set; }

        public Vector(double x, double y, double z) : this()
        {
            X = x;
            Y = y;
            Z = z;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Point
    {
        public float X { get; set; }
        public float Y { get; set; }

        public Point(float x, float y) : this()
        {
            X = x;
            Y = y;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Color
    {
        public float Red { get; set; }
        public float Green { get; set; }
        public float Blue { get; set; }
        public float Alpha { get; set; }

        public Color(float r, float g, float b, float a) : this()
        {
            Red = r;
            Green = g;
            Blue = b;
            Alpha = a;
        }
    }

    public class Render
    {
        public Render()
        {
            Gl.glEnable(Gl.GL_TEXTURE_2D);
            Gl.glEnable(Gl.GL_BLEND);
            Gl.glBlendFunc(Gl.GL_SRC_ALPHA,Gl.GL_ONE_MINUS_SRC_ALPHA);
        }

        public void DrawImmediateModeVertex(Vector position, Color color, Point uvs)
        {
            Gl.glColor4f(color.Red,color.Green,color.Blue,color.Alpha);
            Gl.glTexCoord2f(uvs.X,uvs.Y);
            Gl.glVertex3d(position.X,position.Y,position.Z);
        }

        public void DrawSprite(Sprite sprite)
        {
            Gl.glBegin(Gl.GL_TRIANGLES);
            {
                
            }
            Gl.glEnd();
        }
    }
}
