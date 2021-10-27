import org.w3c.dom.HTMLCanvasElement
import org.khronos.webgl.WebGLRenderingContext as GL
import vision.gears.webglmath.UniformProvider
import vision.gears.webglmath.Vec3
import vision.gears.webglmath.Mat4
import vision.gears.webglmath.Sampler3D
import vision.gears.webglmath.Sampler2D
import kotlin.js.Date

class Scene (
  val gl : WebGL2RenderingContext) : UniformProvider("scene"){

  enum class DisplayMode {
    PHONG, MATCAP, LAYERED
  }

  var displayMode = DisplayMode.PHONG;

  val vsQuad = Shader(gl, GL.VERTEX_SHADER, "shaders/quad-vs.glsl")
  val fsTrace = Shader(gl, GL.FRAGMENT_SHADER, "shaders/trace-fs.glsl") 
  val fsMatcap = Shader(gl, GL.FRAGMENT_SHADER, "shaders/matcap-fs.glsl")
  val fsLayered = Shader(gl, GL.FRAGMENT_SHADER, "shaders/layered-fs.glsl")
  val traceProgram = Program(gl, vsQuad, fsTrace, Program.PNT)
  val matcapProgram = Program(gl, vsQuad, fsMatcap, Program.PNT)
  val layeredProgram = Program(gl, vsQuad, fsLayered, Program.PNT)
  val quadGeometry = TexturedQuadGeometry(gl)  

  val timeAtFirstFrame = Date().getTime()
  var timeAtLastFrame =  timeAtFirstFrame

  val camera = PerspectiveCamera(*Program.all)

  val volumeTexture = Texture3D(gl, "media/brain-at_4096.jpg")
  val matcapTexture = Texture2D(gl, "media/chrome2.jpg")
  val volume = Sampler3D()
  val matcap = Sampler2D()


  init {
    register("volume", volume)
    register("matcapTexture", matcap)
    volume.glTextures[0] = volumeTexture.glTexture
    matcap.glTextures[0] = matcapTexture.glTexture
    addComponentsAndGatherUniforms(*Program.all)
  }


  fun resize(gl : WebGL2RenderingContext, canvas : HTMLCanvasElement) {
    gl.viewport(0, 0, canvas.width, canvas.height)
    camera.setAspectRatio(canvas.width.toFloat() / canvas.height.toFloat())
  }

  @Suppress("UNUSED_PARAMETER")
  fun update(gl : WebGL2RenderingContext, keysPressed : Set<String>) {

    val dt = (Date().getTime().toFloat() - timeAtLastFrame.toFloat()) / 1000.0f
	timeAtLastFrame = Date().getTime()
    camera.move(dt, keysPressed)

    // clear the screen
    gl.clearColor(0.3f, 0.0f, 0.3f, 1.0f)
    gl.clearDepth(1.0f)
    gl.clear(GL.COLOR_BUFFER_BIT or GL.DEPTH_BUFFER_BIT)
    
    if ("J" in keysPressed)
    {
      displayMode = DisplayMode.PHONG;
    }
    else if ("K" in keysPressed)
    {
      displayMode = DisplayMode.MATCAP;
    }
    else if ("L" in keysPressed)
    {
      displayMode = DisplayMode.LAYERED;
    }

    when (displayMode) {
      DisplayMode.PHONG -> traceProgram.draw(this, camera)
      DisplayMode.MATCAP -> matcapProgram.draw(this, camera)
      DisplayMode.LAYERED -> layeredProgram.draw(this, camera)
    }
    quadGeometry.draw()    
  }
}
