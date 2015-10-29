#include <GL/gl.h>


void drawModelWithLocations(Model *m, GLuint program, GLuint positionLocation, GLuint normalLocation, GLuint textureLocation);
{
	glBindVertexArray(m->vao);

	// Position	
	glBindBuffer(GL_ARRAY_BUFFER, m->vb);
	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position_loc);
	
	// Normal
	glBindBuffer(GL_ARRAY_BUFFER, m->nb);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalLocation);
	
	// Texture
	glBindBuffer(GL_ARRAY_BUFFER, m->tb);
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureLocation);
	
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
}
