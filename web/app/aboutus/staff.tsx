import KeyiWang from "@/app/assets/staffphotos/organizers/wang-ky.jpg";
import ZiyiXia from "@/app/assets/staffphotos/organizers/xia-zy.png";
import KentWu from "@/app/assets/staffphotos/organizers/wu-k.jpg";
import EthanHavemann from "@/app/assets/staffphotos/organizers/ethan.jpeg";
import Jiale from "@/app/assets/staffphotos/organizers/jiale.jpeg";

// TODO: fix the photos
import SteveEwald from "@/app/assets/staffphotos/platform/steve.jpeg";
import AndrewLi from "@/app/assets/staffphotos/platform/al.png";
import GavinWang from "@/app/assets/staffphotos/platform/gavin.jpg";
import Nino from "@/app/assets/staffphotos/platform/nino.png";

import XiaoyangLiu from "@/app/assets/staffphotos/advisors/liu-xy.png";
import ZihanDing from "@/app/assets/staffphotos/advisors/ding-zh.png";
import Zhaoran from "@/app/assets/staffphotos/advisors/zhaoran.png";
import { StaticImageData } from "next/image";

interface IStaffMember {
    photo: StaticImageData;
    name: string;
    bio: string;
}

interface IStaffCategory {
    category: string;
    members: IStaffMember[]
}

export const ALL_STAFF_ARRAY: IStaffCategory[] = [
    {
        category: "Organizers",
        members: [
            {
                photo: KeyiWang,
                name: "Keyi Wang",
                bio: "Master's candidate at Northwestern University, bachelor at Columbia University. Interested in machine learning and financial engineering. Core member of AI4Finance open-source community, responsible for project maintenance and promotion, including FinRL and FinRL-Meta.",
            },
            {
                photo: ZiyiXia,
                name: "Ziyi Xia",
                bio: "Master's candidate at Columbia University. Interested in financial reinforcement learning, big data. The core maintainer of AI4Finance open-source community, including popular projects: FinRL, FinRL-Meta.",
            },
            {
                photo: KentWu,
                name: "Kent Wu",
                bio: "Master's candidate at Columbia University with a keen interest in financial reinforcement learning and language modeling. He plays a pivotal role as the primary maintainer of the AI4Finance open-source community. This includes renowned projects such as FinRL and FinRL-Meta.",
            },
            {
                photo: EthanHavemann,
                name: "Ethan Havemann",
                bio: "Undergraduate at Northwestern University, Founder of Northwestern Fintech, and original designer of NUTC. Ethan focuses on software architecture, powerful developer tools, and hardware-up performance optimizations.",
            },
            {
                photo: Jiale,
                name: "Jiale Chen",
                bio: "Undergraduate at Northwestern University and RL lead for Northwestern Fintech. Jiale’s interests lie in applying ML to solve complex, data driven problems.",
            },
        ]
    },
    {
        category: "Platform",
        members: [
            {
                photo: SteveEwald,
                name: "Steve Ewald",
                bio: "Third-year undergraduate student at Northwestern University. Incoming Software Engineer Intern at IMC Trading. Leading research on floating point attack vectors and interactions with chaos theory at the Prescience Lab.",
            },
            {
                photo: AndrewLi,
                name: "Andrew Li",
                bio: "Second-year undergraduate student at Northwestern University doing BA/MS in Computer Science. Incoming intern at Capital One. Interested in developing high performance, impactful software and building infrastructure.",
            },
            {
                photo: GavinWang,
                name: "Gavin Wang",
                bio: "will be written later.",
            },
            {
                photo: Nino,
                name: "Nikola Maruszewski",
                bio: "Nikola Maruszewski is an undergraduate student at Northwestern University majoring in Computer Science, and plans to complete a Masters in Computer Engineering through the BS/MS program. Currently performing research on Quantum Systems with Prof. Nikos Hardavellas in the PARAG@N Lab. Other interests lie in computer hardware, embedded systems, compilers, and operating systems.",
            },
        ]
    },
    {
        category: "Advisors",
        members: [
            {
                photo: XiaoyangLiu,
                name: "Xiao-Yang Liu",
                bio: "Ph.D candidate, Columbia University. His research interests include deep reinforcement learning, big data, and high-performance computing. He co-authored a textbook on reinforcement learning for cyber-physical systems, a textbook on tensor for data processing. He serves as PC member for NeurIPS, ICML, ICLR, AAAI, IJCAI, AISTATS, KDD, ACM MM, ACM ICAIF, ICASSP, and a Session Chair for IJCAI 2019. He organized the NeurIPS 2020/2021 First/Second Workshop on Quantum Tensor Networks in Machine Learning, IJCAI 2020 Workshop on Tensor Networks Representations in Machine Learning, and NeurIPS 2019/2020 Workshop on Machine Learning for Autonomous Driving.",
            },
            {
                photo: ZihanDing,
                name: "Zihan Ding",
                bio: "Ph.D., Princeton University. His primary research interests are: deep reinforcement learning algorithms and applications. He serves as the reviewer of NeurIPS, ICLR, ICML, CVPR, ICCV, AAAI, AISTATS, RA-L, ICRA, IROS, CISS, AIM. He also served on the organization committee of Human in the Loop Learning (HiLL) Workshop at NeurIPS 2022.",
            },
            {
                photo: Zhaoran,
                name: "Zhaoran Wang",
                bio: "Assistant Professor in the Departments of Industrial Engineering & Management Sciences and Computer Science (by courtesy) at Northwestern University (since 2018). He is affiliated with the Centers for Deep Learning and Optimization & Statistical Learning.",
            },
        ]
    },
]