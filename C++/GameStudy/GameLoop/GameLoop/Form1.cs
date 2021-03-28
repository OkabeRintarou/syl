using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Tao.OpenGl;
using Tao.DevIl;

namespace GameLoop
{
    public partial class Form1 : Form
    {
       
        private bool _fullscreen = false;
        private FastLoop _fastloop;
        StateSystem _system = new StateSystem();
        TextureManager _textureManger = new TextureManager();

        public Form1()
        {
            _system.AddState("Splash", new SplashScreenState(_system));
            _system.AddState("title_menu", new TitleMenuState());
            _system.AddState("sprite_test",new DrawSpriteState(_textureManger));
            _system.ChangeState("sprite_test");

            // Init DevIl
            Il.ilInit();
            Ilu.iluInit();
            Ilut.ilutInit();
            Ilut.ilutRenderer(Ilut.ILUT_OPENGL);

            InitializeComponent();
            _openGLControl.InitializeContexts();

            if (_fullscreen)
            {
                FormBorderStyle = FormBorderStyle.None;
                WindowState = FormWindowState.Maximized;
            }
            else
            {
                ClientSize = new Size(800, 600);
            }

            Setup2DGraphics(ClientSize.Width,ClientSize.Height);
            _fastloop = new FastLoop(GameLoop);

            _textureManger.LoadTexture("flower","flower.jpg");
        }

        private void GameLoop(double elapsedTime)
        {
            _system.Update(elapsedTime);
            _system.Render();
            _openGLControl.Refresh();
        }

        private void OnClientSizeChanged(object sender, EventArgs e)
        {
            base.OnClientSizeChanged(e);
            Gl.glViewport(0,0,this.ClientSize.Width,this.ClientSize.Height);
            Setup2DGraphics(ClientSize.Width,ClientSize.Height);
        }

        private void Setup2DGraphics(double width, double height)
        {
            double halfWidth = width/2;
            double halfHeight = height/2;
            Gl.glMatrixMode(Gl.GL_PROJECTION);
            Gl.glLoadIdentity();
            Gl.glOrtho(-halfWidth,halfWidth,-halfHeight,halfHeight,-100,100);
            Gl.glMatrixMode(Gl.GL_MODELVIEW);
            Gl.glLoadIdentity();
        }
        
    }
}
