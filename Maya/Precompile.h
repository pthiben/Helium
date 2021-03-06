#pragma once

// windows
#include "Platform/Windows/Windows.h"

// gl
#include <gl/glu.h>

// stdlib
#include <map>
#include <string>

// maya
#include <maya/M3dView.h>
#include <maya/MAnimControl.h>
#include <maya/MArgList.h>
#include <maya/MBoundingBox.h>
#include <maya/MDGContext.h>
#include <maya/MDagModifier.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MEulerRotation.h>
#include <maya/MFileIO.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MFnCamera.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnIkJoint.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnLight.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSet.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnTransform.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPointArray.h>
#include <maya/MPxCommand.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MPxTransform.h>
#include <maya/MQuaternion.h>
#include <maya/MSelectionList.h>
#include <maya/MStatus.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>

#include <maya/M3dView.h>
#include <maya/MAnimControl.h>
#include <maya/MArgParser.h>
#include <maya/MColor.h>
#include <maya/MDGMessage.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MDistance.h>
#include <maya/MFileIO.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MFnCamera.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnSet.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnTransform.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshEdge.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPxHwShaderNode.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MPxManipContainer.h>
#include <maya/MSelectionList.h>
#include <maya/MSelectionMask.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MVector.h>
#include <maya/MImage.h>
#include <maya/MHwTextureManager.h>

#include <maya/MStatus.h>
#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/M3dView.h>
#include <maya/MFileIO.h>
#include <maya/MSelectionList.h>

#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MDagModifier.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnSingleIndexedComponent.h>

#include <maya/MDGContext.h>
#include <maya/MAnimControl.h>
#include <maya/MFnIkJoint.h>
#include <maya/MFnTransform.h>
#include <maya/MFnCamera.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnSet.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnDirectionalLight.h>

#include <maya/MItDag.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItDependencyNodes.h>

#include <maya/MBoundingBox.h>

#include <maya/MVector.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MPointArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>

#include <maya/MSyntax.h>
#include <maya/MArgDataBase.h>

#ifdef UNICODE
# define asTChar asWChar
#else
# define asTChar asChar
#endif