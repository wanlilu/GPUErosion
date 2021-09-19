#pragma once
#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterMacros.h"
#include "ShaderParameterStruct.h"

class EROSIONCS_API TestShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(TestShader, Global)
	SHADER_USE_PARAMETER_STRUCT(TestShader, FGlobalShader)

public:
	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_TEXTURE(Texture2D<float4>, tmpTexture_input)
		SHADER_PARAMETER_UAV(RWTexture2D<float4>, tmpTexture_out)
		SHADER_PARAMETER(uint32, Index)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(
		const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
	static void ModifyCompilationEnvironment(
		const FGlobalShaderPermutationParameters& Parameters,
		FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	}
};