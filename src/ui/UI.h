#pragma once

namespace UI
{
    enum class Expression
    {
        HAPPY,
        WORRIED,
        SAD,
        SURPRISED
    };

    void begin();

    void setExpression(Expression expression);
}