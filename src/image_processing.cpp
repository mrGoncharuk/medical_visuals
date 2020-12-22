#include "utils.h"


void LoadTextureFromArray(char *pixels, GLuint* out_texture, int image_width, int image_height)
{
    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    // glPixelStorei( GL_UNPACK_ALIGNMENT, 0 );
    glPixelStoref( GL_UNPACK_ALIGNMENT, 1 );
    

    GLfloat swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_RED};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image_width, image_height, 0, GL_RED , GL_UNSIGNED_BYTE, pixels);
    *out_texture = image_texture;
    
}



