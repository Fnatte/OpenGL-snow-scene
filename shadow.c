#include <GL/glew.h>
#include <GL/glut.h>
#include "./libraries/GLUtilities.h"

FBOstruct *fbo;

void initShadowMap() {
	fbo = initFBO(2048, 2048, 0);
	// glBindFramebuffer(GL_FRAMEBUFFER, fbo->fb);
	// glDrawBuffer(GL_NONE);
	// glBindFramebuffer(GL_FRAMEBUFFER, 0L);
}

void beginRenderShadowMap() {
	useFBO(fbo, 0L, 0L);
	
	// Prepare GL state for shadow mapping
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);		
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void endRenderShadowMap() {
	// Restore GL state
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}

FBOstruct* getShadowMapFBO() {
	return fbo;
}

