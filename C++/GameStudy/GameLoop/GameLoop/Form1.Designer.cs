﻿namespace GameLoop
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this._openGLControl = new Tao.Platform.Windows.SimpleOpenGlControl();
            this.SuspendLayout();
            // 
            // _openGLControl
            // 
            this._openGLControl.AccumBits = ((byte)(0));
            this._openGLControl.AutoCheckErrors = false;
            this._openGLControl.AutoFinish = false;
            this._openGLControl.AutoMakeCurrent = true;
            this._openGLControl.AutoSwapBuffers = true;
            this._openGLControl.BackColor = System.Drawing.Color.Black;
            this._openGLControl.ColorBits = ((byte)(32));
            this._openGLControl.DepthBits = ((byte)(16));
            this._openGLControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this._openGLControl.Location = new System.Drawing.Point(0, 0);
            this._openGLControl.Name = "_openGLControl";
            this._openGLControl.Size = new System.Drawing.Size(284, 261);
            this._openGLControl.StencilBits = ((byte)(0));
            this._openGLControl.TabIndex = 0;
            this._openGLControl.Resize += new System.EventHandler(this.OnClientSizeChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 261);
            this.Controls.Add(this._openGLControl);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private Tao.Platform.Windows.SimpleOpenGlControl _openGLControl;
    }
}

