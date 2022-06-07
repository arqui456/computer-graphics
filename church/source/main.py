import glfw
from OpenGL.GL import *
from OpenGL.GL.shaders import compileProgram, compileShader
import pyrr
from TextureLoader import load_texture
from ObjLoader import ObjLoader

from camera import Camera

PI = 3.14159265359

cam = Camera()
WIDTH, HEIGHT = 1280, 720
lastX, lastY = WIDTH / 2, HEIGHT / 2
first_mouse = True
left, right, forward, backward = False, False, False, False
closed_door = True
trigger = False

# the keyboard input callback


def key_input_clb(window, key, scancode, action, mode):
    global left, right, forward, backward, closed_door, trigger
    if key == glfw.KEY_ESCAPE and action == glfw.PRESS:
        glfw.set_window_should_close(window, True)
    if key == glfw.KEY_F and action == glfw.PRESS:
        closed_door = not closed_door
        trigger = True
    if key == glfw.KEY_W and action == glfw.PRESS:
        forward = True
    elif key == glfw.KEY_W and action == glfw.RELEASE:
        forward = False
    if key == glfw.KEY_S and action == glfw.PRESS:
        backward = True
    elif key == glfw.KEY_S and action == glfw.RELEASE:
        backward = False
    if key == glfw.KEY_A and action == glfw.PRESS:
        left = True
    elif key == glfw.KEY_A and action == glfw.RELEASE:
        left = False
    if key == glfw.KEY_D and action == glfw.PRESS:
        right = True
    elif key == glfw.KEY_D and action == glfw.RELEASE:
        right = False


# do the movement, call this function in the main loop
def do_movement():
    if left:
        cam.process_keyboard("LEFT", 0.01)
    if right:
        cam.process_keyboard("RIGHT", 0.01)
    if forward:
        cam.process_keyboard("FORWARD", 0.01)
    if backward:
        cam.process_keyboard("BACKWARD", 0.01)


# the mouse position callback function
def mouse_look_clb(window, xpos, ypos):
    global first_mouse, lastX, lastY

    if first_mouse:
        lastX = xpos
        lastY = ypos
        first_mouse = False

    xoffset = xpos - lastX
    yoffset = lastY - ypos

    lastX = xpos
    lastY = ypos

    cam.process_mouse_movement(xoffset, yoffset)


vertex_src = """
# version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texture;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 v_texture;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    v_texture = a_texture;
}
"""

fragment_src = """
# version 330

in vec2 v_texture;

out vec4 out_color;

uniform sampler2D s_texture;

void main()
{
    out_color = texture2D(s_texture, v_texture);
}
"""


# glfw callback functions
def window_resize(window, width, height):
    glViewport(0, 0, width, height)
    projection = pyrr.matrix44.create_perspective_projection_matrix(
        60, width / height, 0.1, 100)
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, projection)


# initializing glfw library
if not glfw.init():
    raise Exception("glfw can not be initialized!")

# creating the window
window = glfw.create_window(1280, 720, "My OpenGL window", None, None)

# check if window was created
if not window:
    glfw.terminate()
    raise Exception("glfw window can not be created!")

# set window's position
glfw.set_window_pos(window, 400, 200)
# set the callback function for window resize
glfw.set_window_size_callback(window, window_resize)
# set the mouse position callback
glfw.set_cursor_pos_callback(window, mouse_look_clb)
# set the keyboard input callback
glfw.set_key_callback(window, key_input_clb)
# capture the mouse cursor
glfw.set_input_mode(window, glfw.CURSOR, glfw.CURSOR_DISABLED)
# make the context current
glfw.make_context_current(window)

# load here the 3d meshes
church_indices, church_buffer = ObjLoader.load_model("meshes/church2.obj")
door_indices, door_buffer = ObjLoader.load_model("meshes/door.obj")
bench_indices, bench_buffer = ObjLoader.load_model("meshes/bench.obj")
pulpit_indices, pulpit_buffer = ObjLoader.load_model("meshes/pulpit.obj")
window_indices, window_buffer = ObjLoader.load_model("meshes/window.obj")
cross_indices, cross_buffer = ObjLoader.load_model("meshes/cross.obj")
floor_indices, floor_buffer = ObjLoader.load_model("meshes/floor.obj")

shader = compileProgram(compileShader(
    vertex_src, GL_VERTEX_SHADER), compileShader(fragment_src, GL_FRAGMENT_SHADER))

# VAO and VBO
VAO = glGenVertexArrays(7)
VBO = glGenBuffers(7)

# church VAO
glBindVertexArray(VAO[0])
# church Vertex Buffer Object
glBindBuffer(GL_ARRAY_BUFFER, VBO[0])
glBufferData(GL_ARRAY_BUFFER, church_buffer.nbytes,
             church_buffer, GL_STATIC_DRAW)

# church vertices
glEnableVertexAttribArray(0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                      church_buffer.itemsize * 8, ctypes.c_void_p(0))
# church textures
glEnableVertexAttribArray(1)
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                      church_buffer.itemsize * 8, ctypes.c_void_p(12))
# church normals
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                      church_buffer.itemsize * 8, ctypes.c_void_p(20))
glEnableVertexAttribArray(2)

# doors VAO
glBindVertexArray(VAO[1])
# doors Vertex Buffer Object
glBindBuffer(GL_ARRAY_BUFFER, VBO[1])
glBufferData(GL_ARRAY_BUFFER, door_buffer.nbytes,
             door_buffer, GL_STATIC_DRAW)

# doors vertices
glEnableVertexAttribArray(0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                      door_buffer.itemsize * 8, ctypes.c_void_p(0))
# doors textures
glEnableVertexAttribArray(1)
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                      door_buffer.itemsize * 8, ctypes.c_void_p(12))
# doors normals
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                      door_buffer.itemsize * 8, ctypes.c_void_p(20))
glEnableVertexAttribArray(2)

# bench VAO
glBindVertexArray(VAO[2])
# bench Vertex Buffer Object
glBindBuffer(GL_ARRAY_BUFFER, VBO[2])
glBufferData(GL_ARRAY_BUFFER, bench_buffer.nbytes,
             bench_buffer, GL_STATIC_DRAW)

# bench vertices
glEnableVertexAttribArray(0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                      bench_buffer.itemsize * 8, ctypes.c_void_p(0))
# bench textures
glEnableVertexAttribArray(1)
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                      bench_buffer.itemsize * 8, ctypes.c_void_p(12))
# bench normals
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                      bench_buffer.itemsize * 8, ctypes.c_void_p(20))
glEnableVertexAttribArray(2)

# pulpit VAO
glBindVertexArray(VAO[3])
# pulpit Vertex Buffer Object
glBindBuffer(GL_ARRAY_BUFFER, VBO[3])
glBufferData(GL_ARRAY_BUFFER, pulpit_buffer.nbytes,
             pulpit_buffer, GL_STATIC_DRAW)
# pulpit vertices
glEnableVertexAttribArray(0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                      pulpit_buffer.itemsize * 8, ctypes.c_void_p(0))
# pulpit textures
glEnableVertexAttribArray(1)
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                      pulpit_buffer.itemsize * 8, ctypes.c_void_p(12))
# pulpit normals
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                      pulpit_buffer.itemsize * 8, ctypes.c_void_p(20))
glEnableVertexAttribArray(2)

# window VAO
glBindVertexArray(VAO[4])
# window Vertex Buffer Object
glBindBuffer(GL_ARRAY_BUFFER, VBO[4])
glBufferData(GL_ARRAY_BUFFER, window_buffer.nbytes,
             window_buffer, GL_STATIC_DRAW)
# window vertices
glEnableVertexAttribArray(0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                      window_buffer.itemsize * 8, ctypes.c_void_p(0))
# window textures
glEnableVertexAttribArray(1)
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                      window_buffer.itemsize * 8, ctypes.c_void_p(12))
# window normals
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                      window_buffer.itemsize * 8, ctypes.c_void_p(20))
glEnableVertexAttribArray(2)

# cross VAO
glBindVertexArray(VAO[5])
# cross Vertex Buffer Object
glBindBuffer(GL_ARRAY_BUFFER, VBO[5])
glBufferData(GL_ARRAY_BUFFER, cross_buffer.nbytes,
             cross_buffer, GL_STATIC_DRAW)
# cross vertices
glEnableVertexAttribArray(0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                      cross_buffer.itemsize * 8, ctypes.c_void_p(0))
# cross textures
glEnableVertexAttribArray(1)
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                      cross_buffer.itemsize * 8, ctypes.c_void_p(12))
# cross normals
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                      cross_buffer.itemsize * 8, ctypes.c_void_p(20))
glEnableVertexAttribArray(2)

# floor VAO
glBindVertexArray(VAO[6])
# floor Vertex Buffer Object
glBindBuffer(GL_ARRAY_BUFFER, VBO[6])
glBufferData(GL_ARRAY_BUFFER, floor_buffer.nbytes,
             floor_buffer, GL_STATIC_DRAW)
# floor vertices
glEnableVertexAttribArray(0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                      floor_buffer.itemsize * 8, ctypes.c_void_p(0))
# floor textures
glEnableVertexAttribArray(1)
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                      floor_buffer.itemsize * 8, ctypes.c_void_p(12))
# floor normals
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                      floor_buffer.itemsize * 8, ctypes.c_void_p(20))
glEnableVertexAttribArray(2)

textures = glGenTextures(7)
load_texture("meshes/white-wall.jpg", textures[0])
load_texture("meshes/door.jpg", textures[1])
load_texture("meshes/door.jpg", textures[2])
load_texture("meshes/door.jpg", textures[3])
load_texture("meshes/vitrage.jpg", textures[4])
load_texture("meshes/wood3.jpg", textures[5])
load_texture("meshes/floor.jpg", textures[6])

glUseProgram(shader)
glClearColor(0, 0.1, 0.1, 1)
glEnable(GL_DEPTH_TEST)
glEnable(GL_BLEND)
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

projection = pyrr.matrix44.create_perspective_projection_matrix(
    60, 1280 / 720, 0.1, 1000)
church_pos = pyrr.matrix44.create_from_translation(pyrr.Vector3([0, 0, 0]))

doors_pos = pyrr.matrix44.create_from_translation(
    pyrr.Vector3([50, 3.4, -24]))
rot_y = pyrr.Matrix44.from_z_rotation((PI/180) * 180)
doors_pos = pyrr.matrix44.multiply(rot_y, doors_pos)

bench_pos = pyrr.matrix44.create_from_translation(
    pyrr.Vector3([35, -2, -24]))
rot_y = pyrr.Matrix44.from_y_rotation((PI/180) * 90)
bench_pos = pyrr.matrix44.multiply(rot_y, bench_pos)
scale_obj = pyrr.matrix44.create_from_scale(pyrr.Vector3([2, 2, 2]))
bench_pos = pyrr.matrix44.multiply(scale_obj, bench_pos)

pulpit_pos = pyrr.matrix44.create_from_translation(
    pyrr.Vector3([-10, -32, 14]))
scale_obj = pyrr.matrix44.create_from_scale(pyrr.Vector3([3, 3, 3]))
pulpit_pos = pyrr.matrix44.multiply(scale_obj, pulpit_pos)

window_pos = pyrr.matrix44.create_from_translation(
    pyrr.Vector3([0, 9, 18]))

cross_pos = pyrr.matrix44.create_from_translation(pyrr.Vector3([12, -3, 0]))
scale_obj = pyrr.matrix44.create_from_scale(pyrr.Vector3([2, 2, 2]))
cross_pos = pyrr.matrix44.multiply(scale_obj, cross_pos)

floor_pos = pyrr.matrix44.create_from_translation(pyrr.Vector3([0, 0, 0]))
scale_obj = pyrr.matrix44.create_from_scale(pyrr.Vector3([0.1, 0.1, 0.1]))
floor_pos = pyrr.matrix44.multiply(scale_obj, floor_pos)

# eye, target, up
view = pyrr.matrix44.create_look_at(pyrr.Vector3(
    [100, 0, 8]), pyrr.Vector3([0, 0, 0]), pyrr.Vector3([0, 1, 0]))

model_loc = glGetUniformLocation(shader, "model")
proj_loc = glGetUniformLocation(shader, "projection")
view_loc = glGetUniformLocation(shader, "view")

glUniformMatrix4fv(proj_loc, 1, GL_FALSE, projection)
glUniformMatrix4fv(view_loc, 1, GL_FALSE, view)

step = 1 * (PI/180)

# the main application loop
while not glfw.window_should_close(window):
    glfw.poll_events()
    do_movement()
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    view = cam.get_view_matrix()
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, view)

    # draw the church character
    model = church_pos
    glBindVertexArray(VAO[0])
    glBindTexture(GL_TEXTURE_2D, textures[0])
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)
    glDrawArrays(GL_TRIANGLES, 0, len(church_indices))

    # draw the doors
    model = doors_pos
    for door in range(3):
        trans = pyrr.matrix44.create_from_translation(
            pyrr.Vector3([0, 0, 12]))
        model = pyrr.matrix44.multiply(trans, model)
        if door == 2 and not closed_door and trigger:
            if step < (90 * (PI/180)):
                rot_y = pyrr.Matrix44.from_y_rotation(step)
                model = pyrr.matrix44.multiply(rot_y, model)
                step += 0.1 * (PI/180)
            else:
                trigger = False
        elif door == 2 and closed_door and trigger:
            if step > (0 * (PI/180)):
                rot_y = pyrr.Matrix44.from_y_rotation(step)
                model = pyrr.matrix44.multiply(rot_y, model)
                step -= 0.1 * (PI/180)
            else:
                trigger = False
        if door == 2 and not trigger:
            model = pyrr.matrix44.multiply(rot_y, model)
        glBindVertexArray(VAO[1])
        glBindTexture(GL_TEXTURE_2D, textures[1])
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)
        glDrawArrays(GL_TRIANGLES, 0, len(door_indices))

    # draw the benchs
    model = bench_pos
    for x in range(2):
        trans = pyrr.matrix44.create_from_translation(pyrr.Vector3([8, 0, 0]))
        model = pyrr.matrix44.multiply(trans, model)
        glBindVertexArray(VAO[2])
        glBindTexture(GL_TEXTURE_2D, textures[2])
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)
        glDrawArrays(GL_QUADS, 0, len(bench_indices))
    trans = pyrr.matrix44.create_from_translation(pyrr.Vector3([-16, 0, 0]))
    model = pyrr.matrix44.multiply(trans, model)
    trans = pyrr.matrix44.create_from_translation(pyrr.Vector3([0, 0, 3]))
    model = pyrr.matrix44.multiply(trans, model)
    for x in range(2):
        trans = pyrr.matrix44.create_from_translation(pyrr.Vector3([8, 0, 0]))
        model = pyrr.matrix44.multiply(trans, model)
        glBindVertexArray(VAO[2])
        glBindTexture(GL_TEXTURE_2D, textures[2])
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)
        glDrawArrays(GL_QUADS, 0, len(bench_indices))
    trans = pyrr.matrix44.create_from_translation(pyrr.Vector3([-16, 0, 0]))
    model = pyrr.matrix44.multiply(trans, model)
    trans = pyrr.matrix44.create_from_translation(pyrr.Vector3([0, 0, 3]))
    model = pyrr.matrix44.multiply(trans, model)
    for x in range(2):
        trans = pyrr.matrix44.create_from_translation(
            pyrr.Vector3([8, 0, 0]))
        model = pyrr.matrix44.multiply(trans, model)
        glBindVertexArray(VAO[2])
        glBindTexture(GL_TEXTURE_2D, textures[2])
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)
        glDrawArrays(GL_QUADS, 0, len(bench_indices))

    # draw the church pulpit
    model = pulpit_pos
    glBindVertexArray(VAO[3])
    glBindTexture(GL_TEXTURE_2D, textures[3])
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)
    glDrawArrays(GL_TRIANGLES, 0, len(pulpit_indices))

    # draw the windows
    model = window_pos
    for _ in range(4):
        trans = pyrr.matrix44.create_from_translation(
            pyrr.Vector3([10, 0, 0]))
        model = pyrr.matrix44.multiply(trans, model)
        glBindVertexArray(VAO[4])
        glBindTexture(GL_TEXTURE_2D, textures[4])
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)
        glDrawArrays(GL_TRIANGLES, 0, len(window_indices))

    trans = pyrr.matrix44.create_from_translation(
        pyrr.Vector3([0, 0, -37]))
    model = pyrr.matrix44.multiply(trans, model)
    trans = pyrr.matrix44.create_from_translation(
        pyrr.Vector3([10, 0, 0]))
    model = pyrr.matrix44.multiply(trans, model)
    for _ in range(4):
        trans = pyrr.matrix44.create_from_translation(
            pyrr.Vector3([-10, 0, 0]))
        model = pyrr.matrix44.multiply(trans, model)
        glBindVertexArray(VAO[4])
        glBindTexture(GL_TEXTURE_2D, textures[4])
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)
        glDrawArrays(GL_TRIANGLES, 0, len(window_indices))

    # draw the church cross
    model = cross_pos
    glBindVertexArray(VAO[5])
    glBindTexture(GL_TEXTURE_2D, textures[5])
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)
    glDrawArrays(GL_TRIANGLES, 0, len(cross_indices))

    # draw the church floor
    model = floor_pos
    glBindVertexArray(VAO[6])
    glBindTexture(GL_TEXTURE_2D, textures[6])
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model)
    glDrawArrays(GL_TRIANGLES, 0, len(floor_indices))

    glfw.swap_buffers(window)
# terminate glfw, free up allocated resources
glfw.terminate()
