#pragma once

#include <vsg/nodes/Node.h>

#include <vsg/vk/GraphicsPipeline.h>
#include <vsg/vk/PushConstants.h>
#include <vsg/vk/CommandPool.h>


namespace vsg
{
    struct Context
    {
        vsg::ref_ptr<vsg::Device> device;
        vsg::ref_ptr<vsg::CommandPool> commandPool;
        vsg::ref_ptr<vsg::RenderPass> renderPass;
        vsg::ref_ptr<vsg::ViewportState> viewport;
        VkQueue graphicsQueue;
    };

    class GraphicsPipelineConfig : public Inherit<Object, GraphicsPipelineConfig>
    {
    public:
        GraphicsPipelineConfig(Allocator* allocator = nullptr);

        GraphicsPipeline* createPipeline(Device* device, RenderPass* renderPass, ViewportState* viewport);

        using Shaders = std::vector<ref_ptr<Shader>>;

        // descriptorPool ..
        uint32_t maxSets = 0;
        DescriptorPoolSizes descriptorPoolSizes;
        // descriptorSetLayout ..
        vsg::DescriptorSetLayoutBindings descriptorSetLayoutBindings;
        vsg::PushConstantRanges pushConstantRanges;
        vsg::VertexInputState::Bindings vertexBindingsDescriptions;
        vsg::VertexInputState::Attributes vertexAttributeDescriptions;
        Shaders shaders;
        GraphicsPipelineStates pipelineStates;
    };


    // compilable?
    class Geometry;
    class MatrixTransform : public Inherit<Group, MatrixTransform>
    {
    public:
        MatrixTransform(Allocator* allocator = nullptr);

        void accept(DispatchTraversal& dv) const override;

        dmat4 _matrix;

        ref_ptr<StateGroup> _stateGroup;

        void compile(Context& context);
    };

    class Geometry : public Inherit<Group, Geometry>
    {
    public:
        Geometry(Allocator* allocator = nullptr);

        void accept(DispatchTraversal& dv) const override;

        using Commands = std::vector<ref_ptr<Command>>;

        DataList _arrays;
        ref_ptr<Data> _indices;
        Commands _commands;

        ref_ptr<StateGroup> _stateGroup;

        void compile(Context& context);

    };

    class StateSet : public Inherit<Group, StateSet>
    {
    public:
        StateSet(Allocator* allocator = nullptr);

        void accept(DispatchTraversal& dv) const override;

        void compile(Context& context);

        ref_ptr<Data> textureData;
        vsg::ref_ptr<vsg::Shader> vertexShader;
        vsg::ref_ptr<vsg::Shader> fragmentShader;

    };

    class CompileTraversal : public Visitor
    {
    public:

        CompileTraversal() {}

        void apply(Group& group);

        void apply(Geometry& geometry);
        void apply(MatrixTransform& transform);
        void apply(StateSet& stateset);

        Context context;
    };
}

extern vsg::ref_ptr<vsg::Node> createRawSceneData(vsg::Paths& searchPaths);
