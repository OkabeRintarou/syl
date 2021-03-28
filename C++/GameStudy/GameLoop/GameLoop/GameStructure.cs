using System;
using System.Collections.Generic;
using Tao.OpenGl;

namespace GameLoop
{
    public interface IGameObject
    {
        void Update(double elapsedTime);
        void Render();
    }

    class SplashScreenState:IGameObject
    {
        private StateSystem _system;
        private double _delayInSeconds = 3;

        public SplashScreenState(StateSystem system)
        {
            _system = system;
        }

        #region IGameObject Functions
        public void Update(double elapsedTime)
        {
            _delayInSeconds -= elapsedTime;
            if (_delayInSeconds <= 0)
            {
                _delayInSeconds = 3;
                _system.ChangeState("title_menu");
            }
        }

        public void Render()
        {
            Gl.glClearColor(1,1,1,1);
            Gl.glClear(Gl.GL_COLOR_BUFFER_BIT);
            Gl.glFinish();
        }

        #endregion

    }

    class TitleMenuState : IGameObject
    {
        private double _currentRotation;

        public TitleMenuState()
        {
            _currentRotation = 0.0;
        }

        #region IGameObject Functions
        void IGameObject.Update(double elapsedTime)
        {
            _currentRotation = 10 * elapsedTime;

            if (_currentRotation >= 360.0)
            {
                _currentRotation -= 360;
            }
        }

        void IGameObject.Render()
        {
            Gl.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            Gl.glClear(Gl.GL_COLOR_BUFFER_BIT);

            Gl.glRotated(_currentRotation, 1, 0, 0);
            Gl.glRotated(_currentRotation, 0, 1, 0);
            Gl.glRotated(_currentRotation, 0, 0, 1);
            Gl.glBegin(Gl.GL_TRIANGLE_STRIP);
            {
                Gl.glColor4d(1.0, 0.0, 0.0, 0.5);
                Gl.glVertex3d(-150, 0.0, 0.0);
                Gl.glColor3d(0.0, 1.0, 0.0);
                Gl.glVertex3d(150, 0.0, 0.0);
                Gl.glColor3d(0.0, 0.0, 1.0);
                Gl.glVertex3d(0.0, 150, 0.0);
            }
            Gl.glEnd();
            Gl.glFinish();
        }

        #endregion
    }
    
    class StateSystem
    {
        private readonly Dictionary<String, IGameObject> _stateStore;
        private IGameObject _currentState;

        public StateSystem()
        {
            _stateStore = new Dictionary<string, IGameObject>();
            _currentState = null;
        }

        public void Update(double elapsedTime)
        {
            if (_currentState == null)
            {
                return;
            }
            _currentState.Update(elapsedTime);
        }

        public void Render()
        {
            if (_currentState == null)
            {
                return;
            }

            _currentState.Render();
        }

        public void AddState(String stateId, IGameObject state)
        {
            System.Diagnostics.Debug.Assert(Exists(stateId) == false);
            _stateStore.Add(stateId,state);
        }

        public void ChangeState(String stateId)
        {
            System.Diagnostics.Debug.Assert(Exists(stateId));
            _currentState = _stateStore[stateId];
        }
        private bool Exists(string stateId)
        {
            return _stateStore.ContainsKey(stateId);
        }
    }

    class DrawSpriteState : IGameObject
    {
        private TextureManager _textureManager;

        #region IGameObject Members

        public DrawSpriteState(TextureManager textureManager)
        {
            _textureManager = textureManager;
        }
          
        public void Update(double elapsedTime)
        {

        }

        public void Render()
        {
            Texture texture = _textureManager.Get("flower");
            Gl.glEnable(Gl.GL_TEXTURE_2D);
            Gl.glBindTexture(Gl.GL_TEXTURE_2D,texture.Id);

            double width = 600;
            double height = 600;
            double halfwidth = width/2;
            double halfheight = height/2;

            double x = 0;
            double y = 0;
            double z = 0;

            float topUV = 0;
            float bottomUV = 1.0f;
            float leftUV = 0;              
            float rightUV = 1.0f;

            float red = 1;
            float green = 0;
            float blue = 0;
            float alpha = 0;

            //alpha += 0.1f;
            //if (alpha >= 1.0f)
            //{
            //    alpha = 0.0f;
            //}

            Gl.glBegin(Gl.GL_TRIANGLES);
            {
                Gl.glColor4d(red,green,blue,alpha);
                Gl.glTexCoord2d(leftUV, topUV);
                Gl.glVertex3d(x - halfwidth,y+halfheight,z);
                Gl.glTexCoord2d(rightUV, topUV);
                Gl.glVertex3d(x + halfwidth,y+halfheight,z);
                Gl.glTexCoord2d(leftUV,bottomUV);
                Gl.glVertex3d(x - halfwidth, y - halfheight, z);

                Gl.glTexCoord2d(rightUV, topUV);
                Gl.glVertex3d(x + halfwidth, y + halfheight,z);
                Gl.glTexCoord2d(rightUV, bottomUV);
                Gl.glVertex3d(x + halfwidth,y -halfheight,z);
                Gl.glTexCoord2d(leftUV, bottomUV);
                Gl.glVertex3d(x - halfwidth,y - halfheight,z);
            }
            Gl.glEnd();
        }

        #endregion
    }
}
