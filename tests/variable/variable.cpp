/*
Copyright 2015 University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.Some license of other
*/

#include "gtest/gtest.h"

#include <libcellml>


TEST(Variable, serialise) {
    const std::string e = "<variable/>";
    libcellml::Variable v;
    std::string a = v.serialise(libcellml::FORMAT_XML);
    EXPECT_EQ(e, a);
}

TEST(Variable, setValidVariableName) {
    const std::string in = "valid_name";
    const std::string e = "<variable name=\"" + in + "\"/>";
    libcellml::Variable v;
    v.setName(in);
    std::string a = v.serialise(libcellml::FORMAT_XML);
    EXPECT_EQ(e, a);
}

TEST(Variable, setInvalidVariableName) {
    const std::string in = "invalid name";
    const std::string e = "<variable name=\"" + in + "\"/>";
    libcellml::Variable v;
    v.setName(in);
    std::string a = v.serialise(libcellml::FORMAT_XML);
    EXPECT_EQ(e, a);
}

TEST(Variable, getValidVariableName) {
    const std::string in = "valid_name";
    const std::string e = in;
    libcellml::Variable v;
    v.setName(in);
    std::string a = v.getName();
    EXPECT_EQ(e, a);
}

TEST(Variable, getInvalidVariableName) {
    const std::string in = "invalid name";
    const std::string e = in;
    libcellml::Variable v;
    v.setName(in);
    std::string a = v.getName();
    EXPECT_EQ(e, a);
}

TEST(Variable, setUnits) {
    const std::string in = "dimensionless";
    const std::string e = "<variable units=\"" + in + "\"/>";
    libcellml::Variable v;

    libcellml::UnitsPtr u = std::make_shared<libcellml::Units>();
    u->setName(in);
    v.setUnits(u);

    std::string a = v.serialise(libcellml::FORMAT_XML);
    EXPECT_EQ(e, a);
}

TEST(Variable, setUnitsAndName) {
    const std::string in = "valid_name";
    const std::string e = "<variable name=\"" + in + "\" units=\"dimensionless\"/>";
    libcellml::Variable v;
    v.setName(in);

    libcellml::UnitsPtr u = std::make_shared<libcellml::Units>();
    u->setName("dimensionless");
    v.setUnits(u);

    std::string a = v.serialise(libcellml::FORMAT_XML);
    EXPECT_EQ(e, a);
}

TEST(Variable, addVariable) {
    const std::string in = "valid_name";
    const std::string e =
            "<component name=\"" + in + "\">"
                "<variable name=\"" + in + "\" units=\"dimensionless\"/>"
              "</component>";

    libcellml::Component c;
    c.setName(in);

    libcellml::VariablePtr v = std::make_shared<libcellml::Variable>();
    v->setName(in);
    c.addVariable(v);

    libcellml::UnitsPtr u = std::make_shared<libcellml::Units>();
    u->setName("dimensionless");
    v->setUnits(u);

    std::string a = c.serialise(libcellml::FORMAT_XML);
    EXPECT_EQ(e, a);
}

TEST(Variable, addTwoVariables) {
    const std::string in = "valid_name";
    const std::string e =
            "<component name=\"" + in + "\">"
                "<variable name=\"variable1\"/>"
                "<variable name=\"variable2\"/>"
            "</component>";

    libcellml::Component c;
    c.setName(in);

    libcellml::VariablePtr v1 = std::make_shared<libcellml::Variable>();
    v1->setName("variable1");
    c.addVariable(v1);

    libcellml::VariablePtr v2 = std::make_shared<libcellml::Variable>();
    v2->setName("variable2");
    c.addVariable(v2);

    std::string a = c.serialise(libcellml::FORMAT_XML);
    EXPECT_EQ(e, a);
}

TEST(Variable, removeVariable) {
    const std::string in = "valid_name";
    const std::string e =
            "<component name=\"" + in + "\">"
                "<variable name=\"variable2\"/>"
            "</component>";

    libcellml::Component c;
    c.setName(in);

    libcellml::VariablePtr v1 = std::make_shared<libcellml::Variable>();
    v1->setName("variable1");
    c.addVariable(v1);

    libcellml::VariablePtr v2 = std::make_shared<libcellml::Variable>();
    v2->setName("variable2");
    c.addVariable(v2);

    c.removeVariable("variable1");
    std::string a = c.serialise(libcellml::FORMAT_XML);
    EXPECT_EQ(e, a);

    EXPECT_THROW(c.removeVariable("BAD_NAME"), std::out_of_range);
}

TEST(Variable, getVariableMethods) {
    const std::string in = "valid_name";
    libcellml::Component c;
    c.setName(in);

    libcellml::VariablePtr v1 = std::make_shared<libcellml::Variable>();
    v1->setName("variable1");
    c.addVariable(v1);
    libcellml::VariablePtr v2 = std::make_shared<libcellml::Variable>();
    v2->setName("variable2");
    c.addVariable(v2);
    libcellml::VariablePtr v3 = std::make_shared<libcellml::Variable>();
    v3->setName("variable3");
    c.addVariable(v3);
    libcellml::VariablePtr v4 = std::make_shared<libcellml::Variable>();
    v4->setName("variable4");
    c.addVariable(v4);

    // Get by string
    libcellml::VariablePtr vMethod1 = c.getVariable("variable1");
    std::string a1 = vMethod1->getName();
    EXPECT_EQ("variable1", a1);

    // Get by index
    libcellml::VariablePtr vMethod2 = c.getVariable(1);
    std::string a2 = vMethod2->getName();
    EXPECT_EQ("variable2", a2);

    // Get const by string
    const libcellml::VariablePtr vMethod3 = static_cast<const libcellml::Component>(c).getVariable("variable3");
    std::string a3 = vMethod3->getName();
    EXPECT_EQ("variable3", a3);

    // Get const by index
    const libcellml::VariablePtr vMethod4 = static_cast<const libcellml::Component>(c).getVariable(3);
    std::string a4 = vMethod4->getName();
    EXPECT_EQ("variable4", a4);
}

TEST(Variable, modelValidVariableName) {
    const std::string in = "valid_name";
    const std::string e =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<model xmlns=\"http://www.cellml.org/cellml/1.2#\">"
              "<component name=\"" + in + "\">"
                "<variable name=\"" + in + "\" units=\"dimensionless\"/>"
              "</component>"
            "</model>";

    libcellml::Model m;

    libcellml::ComponentPtr c = std::make_shared<libcellml::Component>();
    c->setName(in);
    m.addComponent(c);

    libcellml::VariablePtr v = std::make_shared<libcellml::Variable>();
    v->setName(in);
    c->addVariable(v);

    libcellml::UnitsPtr u = std::make_shared<libcellml::Units>();
    u->setName("dimensionless");
    v->setUnits(u);

    std::string a = m.serialise(libcellml::FORMAT_XML);
    EXPECT_EQ(e, a);
    EXPECT_EQ("valid_name", v->getName());
}

TEST(Variable, modelInvalidVariableName) {
    const std::string in = "invalid name";
    const std::string e =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<model xmlns=\"http://www.cellml.org/cellml/1.2#\">"
              "<component name=\"" + in + "\">"
                "<variable name=\"" + in + "\" units=\"dimensionless\"/>"
              "</component>"
            "</model>";

    libcellml::Model m;

    libcellml::ComponentPtr c = std::make_shared<libcellml::Component>();
    c->setName(in);
    m.addComponent(c);

    libcellml::VariablePtr v = std::make_shared<libcellml::Variable>();
    v->setName(in);
    c->addVariable(v);

    libcellml::UnitsPtr u = std::make_shared<libcellml::Units>();
    u->setName("dimensionless");
    v->setUnits(u);

    std::string a = m.serialise(libcellml::FORMAT_XML);
    EXPECT_EQ(e, a);
    EXPECT_EQ("invalid name", v->getName());
}

TEST(Variable, modelInvalidUnitsName) {
    const std::string in = "valid_name";
    const std::string e =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<model xmlns=\"http://www.cellml.org/cellml/1.2#\">"
              "<component name=\"" + in + "\">"
                "<variable name=\"" + in + "\" units=\"invalid name\"/>"
              "</component>"
            "</model>";

    libcellml::Model m;

    libcellml::ComponentPtr c = std::make_shared<libcellml::Component>();
    c->setName(in);
    m.addComponent(c);

    libcellml::VariablePtr v = std::make_shared<libcellml::Variable>();
    v->setName(in);
    c->addVariable(v);

    libcellml::UnitsPtr u = std::make_shared<libcellml::Units>();
    u->setName("invalid name");
    v->setUnits(u);

    std::string a = m.serialise(libcellml::FORMAT_XML);
    EXPECT_EQ(e, a);
    EXPECT_EQ("invalid name", u->getName());
}


