using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Tao.DevIl;
using Tao.OpenGl;

namespace GameLoop
{
    public struct Texture
    {
        public Int32 Id { get; set; }
        public Int32 Width { get; set; }
        public Int32 Height { get; set; }

        public Texture(Int32 id, Int32 width, Int32 height) : this()
        {
            Id = id;
            Width = width;
            Height = height;
        }
    }

    class TextureManager : IDisposable
    {
        Dictionary<String,Texture> _textureDatabase = new Dictionary<String, Texture>();

        public Texture Get(String textureId)
        {
            return _textureDatabase[textureId];
        }

        #region IDisposable Members

        public void Dispose()
        {
            foreach (Texture t in _textureDatabase.Values)
            {
                Gl.glDeleteTextures(1,new int[] {t.Id});
            }
        }

        public void LoadTexture(String textureId, String path)
        {
            int devilId = 0;
            Il.ilGenImages(1,out devilId);
            Il.ilBindImage(devilId);

            if (!Il.ilLoadImage(path))
            {
                System.Diagnostics.Debug.Assert(false,
                    "Could not open file,[ " + path + "].");
            }

            Ilu.iluFlipImage();
            int width = Il.ilGetInteger(Il.IL_IMAGE_WIDTH);
            int height = Il.ilGetInteger(Il.IL_IMAGE_HEIGHT);
            int openGLId = Ilut.ilutGLBindTexImage();

            System.Diagnostics.Debug.Assert(openGLId != 0);
            Il.ilDeleteImages(1,ref devilId);

            _textureDatabase.Add(textureId,new Texture(openGLId,width,height));

        }
        #endregion
    }
}
