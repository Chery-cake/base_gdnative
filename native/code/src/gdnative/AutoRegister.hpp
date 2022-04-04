#ifndef SRC_GDNATIVE_CLASS_AUTOREGISTER_HPP_
#define SRC_GDNATIVE_CLASS_AUTOREGISTER_HPP_

#include <Godot.hpp>
#include <Engine.hpp>
#include <vector>
#include <string>
#include <fstream>

#define REGISTER_CLASS(MAIN_CLASS, BASE_CLASS) static inline bool m_registered = AutoRegister::register_class<MAIN_CLASS>(#MAIN_CLASS);\
	GODOT_CLASS(MAIN_CLASS, BASE_CLASS);
#define REGISTER_TOOL(MAIN_CLASS, BASE_CLASS) static inline bool m_registered = AutoRegister::register_tool<MAIN_CLASS>(#MAIN_CLASS);\
	GODOT_CLASS(MAIN_CLASS, BASE_CLASS);

class AutoRegister{
private:

	static std::vector<std::string>& m_names() {
		static std::vector<std::string> instance;
		return instance;
	}

	static std::vector<void (*)()>& m_class_funcs() {
		static std::vector<void (*)()> instance;
		return instance;
	}

	static std::vector<void (*)()>& m_tool_funcs() {
		static std::vector<void (*)()> instance;
		return instance;
	}

public:

	AutoRegister() = delete;

	template<typename T>
	static inline bool register_class(const std::string main_type) {

		m_class_funcs().push_back([] {
			godot::register_class<T>();
		});

		m_names().push_back(main_type);

		return true;
	}

	template<typename T>
	static inline bool register_tool(const std::string main_type) {

		m_class_funcs().push_back([] {
			godot::register_tool_class<T>();
		});

		m_names().push_back(main_type);

		return true;
	}

	static void register_classes() {
		for (auto func : m_class_funcs()) {
			func();
		}
		m_class_funcs().clear();

		for (auto func : m_tool_funcs()) {
			func();
		}
		m_tool_funcs().clear();
	}

	static void generate_gdns(const std::string &output_path,
			const std::string &lib_name) {
		bool editor = godot::Engine::get_singleton()->is_editor_hint();
		if (editor) {
			std::fstream gdnlib_file(lib_name + ".gdnlib");
			if (gdnlib_file.fail()) {
				return;
			}
			for (std::string name : m_names()) {
				std::fstream gdns(output_path + "/" + name + ".gdns");
				if (!gdns.fail()) {
					godot::Godot::print(
							(std::string("gdns ") + name
									+ " already exists. skipping...").c_str());
					continue;
				}

				gdns.open(output_path + "/" + name + ".gdns", std::ios::out);
				gdns
						<< "[gd_resource type=\"NativeScript\" load_steps=2 format=2]"
						<< std::endl << "[ext_resource path=\"res://"
						<< lib_name
						<< ".gdnlib\" type=\"GDNativeLibrary\" id=1]" << std::endl
						<< "[resource]" << std::endl << "resource_name = \""
						<< name << "\"" << std::endl << "class_name = \""
						<< name << "\"" << std::endl
						<< "library = ExtResource( 1 )" << std::endl;

				godot::Godot::print((std::string("generated gdns ") + name).c_str());

			}
		}
	}
};

#endif /* SRC_GDNATIVE_CLASS_AUTOREGISTER_HPP_ */
