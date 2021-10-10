
#include "CustomNodeScene.h"
#include "renderer/backend/ProgramState.h"
#include "renderer/backend/Device.h"
#include "renderer/CCCustomCommand.h"
#include "math/CCMath.h"

USING_NS_CC;

namespace rtv {

namespace {

class CircleNode: public Node {
public:
    static CircleNode* create(float radius) {
        CircleNode* ret = new (std::nothrow) CircleNode(radius);
        if (ret && ret->init())
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(ret);
        }
        
        return ret;
    }
    
    virtual bool init() override {
        if (!Node::init()) {
            return false;
        }
            
        auto* program = backend::Program::getBuiltinProgram(backend::ProgramType::POSITION_COLOR);
        _programState = new (std::nothrow) backend::ProgramState(program);
        _customCommand.getPipelineDescriptor().programState = _programState;
        setVertexLayout(_customCommand);
        _customCommand.setDrawType(CustomCommand::DrawType::ARRAY);
        _customCommand.setPrimitiveType(CustomCommand::PrimitiveType::TRIANGLE);

        createVertices();
        
        return true;
    }
    
    void setVertexLayout(CustomCommand& cmd)
    {
        auto* programState = cmd.getPipelineDescriptor().programState;
        auto layout = programState->getVertexLayout();
        const auto& attributeInfo = programState->getProgram()->getActiveAttributes();
        auto iter = attributeInfo.find("a_position");
        if(iter != attributeInfo.end())
        {
            layout->setAttribute("a_position", iter->second.location, backend::VertexFormat::FLOAT2, 0, false);
        }
        
        iter = attributeInfo.find("a_texCoord");
        if(iter != attributeInfo.end())
        {
            layout->setAttribute("a_texCoord", iter->second.location, backend::VertexFormat::FLOAT2, offsetof(V2F_C4B_T2F, texCoords), false);
        }
        
        iter = attributeInfo.find("a_color");
        if(iter != attributeInfo.end())
        {
            layout->setAttribute("a_color", iter->second.location, backend::VertexFormat::UBYTE4, offsetof(V2F_C4B_T2F, colors), true);
        }
        layout->setLayout(sizeof(V2F_C4B_T2F));
    }

    void createVertices() {
        std::vector<V2F_C4B_T2F> vertices;
        
        static constexpr float segments = 30.0f;
        
        float angleDelta = 2.0f * M_PI / segments;
        
        auto angle = 0.0f;
        
        for (int i=0; i<segments; ++i) {
            vertices.push_back({Vec2{0, 0}, Color4B{255, 255, 0, 255}, Tex2F{0, 0}});
            vertices.push_back({Vec2{std::cosf(angle), std::sinf(angle)} * mRadius,
                Color4B{0, 255, 255, 255}, Tex2F{0, 0}});
            angle += angleDelta;
            vertices.push_back({Vec2{std::cosf(angle), std::sinf(angle)} * mRadius,
                Color4B{255, 0, 255, 255}, Tex2F{0, 0}});
        }
        
        _customCommand.createVertexBuffer(sizeof(V2F_C4B_T2F),
                                          vertices.size(),
                                          CustomCommand::BufferUsage::STATIC);
        
        _customCommand.updateVertexBuffer(vertices.data(), 0, vertices.size()*sizeof(V2F_C4B_T2F));
        _customCommand.setVertexDrawInfo(0, vertices.size());
    }
    
    void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override
    {
        updateUniforms(transform, _customCommand);
        _customCommand.init(_globalZOrder);
        renderer->addCommand(&_customCommand);
    }
    
    void updateUniforms(const Mat4 &transform, CustomCommand& cmd)
    {
        auto& pipelineDescriptor = cmd.getPipelineDescriptor();
        const auto& matrixP = _director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
        Mat4 matrixMVP = matrixP * transform;
        auto mvpLocation = pipelineDescriptor.programState->getUniformLocation("u_MVPMatrix");
        pipelineDescriptor.programState->setUniform(mvpLocation, matrixMVP.m, sizeof(matrixMVP.m));
    }

protected:
    CircleNode(float radius) :
        mRadius{radius} {
    }
    
    CustomCommand _customCommand;
    backend::ProgramState* _programState{nullptr};
    float mRadius{ 0 };
};

};

Scene* CustomNodeScene::createScene() {
    return CustomNodeScene::create();
}

bool CustomNodeScene::init() {
    if (!ExperimentScene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto circleNode = CircleNode::create(100.0f);
    float x = visibleSize.width/2 - circleNode->getContentSize().width/2;
    float y = visibleSize.height/2 - circleNode->getContentSize().height/2;
    circleNode->setPosition(Vec2(x, y));
    circleNode->setScale(2.0f);
    this->addChild(circleNode);
    
    return true;
}

}
